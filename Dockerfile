# Make: https://www.gnu.org/software/make
FROM debian:bullseye-slim AS stage-make

ADD https://ftp.gnu.org/gnu/make/make-4.3.tar.gz /make.tar.gz
WORKDIR /make-build

# The configuration steps yields an error without `--disable-dependency-tracking`.
# Make requires itself to build and install. So using `build.sh` which produces a minimal `make` that can then be used
# to build and install the definitive `make` under `/make`. That can be later copied to a location like `/usr/local/`.
RUN \
    apt update && apt install --yes \
        gcc \
    && tar -xzf /make.tar.gz --directory=/make-build --strip-components=1 \
    && mkdir /make \
    && ./configure --disable-dependency-tracking --prefix=/make \
    && ./build.sh \
    && ./make \
    && ./make install

# CMake: https://cmake.org/download
FROM debian:bullseye-slim AS stage-cmake

ADD https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2-linux-x86_64.tar.gz /cmake.tar.gz

RUN \
    mkdir /cmake \
    && tar -xzf /cmake.tar.gz --directory=/cmake --strip-components=1

# IniPP: https://github.com/mcmtroffaes/inipp
FROM debian:bullseye-slim as stage-inipp

ADD https://raw.githubusercontent.com/mcmtroffaes/inipp/46248e4e93a2e63f9a1d0d8d9ad40bd6b3725df5/inipp/inipp.h /usr/local/include/
# Note that `ADD` has `--chmod` in BuildKit.
RUN chmod 644 /usr/local/include/inipp.h

# SDL: https://wiki.libsdl.org/Installation
FROM debian:bullseye-slim AS stage-sdl

ADD https://github.com/libsdl-org/SDL/releases/download/release-2.0.20/SDL2-2.0.20.tar.gz /SDL.tar.gz
WORKDIR /sdl-build

RUN \
    apt update \
    && apt install --yes \
        gcc \
        make \
    && tar -xzf /SDL.tar.gz --directory=/sdl-build --strip-components=1 \
    && mkdir /sdl \
    && ./configure --prefix=/sdl \
    && make \
    && make install

# Clang: https://apt.llvm.org
FROM debian:bullseye-slim AS stage-clang

RUN \
    apt update \
    && apt install --yes \
        gnupg \
        wget \
    && wget --output-document=- https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - \
    && echo "deb http://apt.llvm.org/bullseye llvm-toolchain-bullseye-13 main" > /etc/apt/sources.list.d/llvm.list \
    && echo "deb-src http://apt.llvm.org/bullseye llvm-toolchain-bullseye-13 main" >> /etc/apt/sources.list.d/llvm.list \
    && apt update \
    && apt install --yes \
        clang-13 \
        clang-format-13 \
        clang-tidy-13 \
    && update-alternatives --install /usr/bin/clang clang /usr/bin/clang-13 60 \
        --slave /usr/bin/clang++ clang++ /usr/bin/clang++-13 \
        --slave /usr/bin/clang-format clang-format /usr/bin/clang-format-13 \
        --slave /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-13 \
    && apt autoremove --purge --yes \
        gnupg \
        wget \
    && rm -rf /var/lib/apt/lists/*

# GCC: https://stackoverflow.com/a/61591646
FROM stage-clang AS stage-compilers

RUN \
    apt update \
    && apt install --yes \
        g++ \
    && rm -rf /var/lib/apt/lists/*

# Final image
FROM stage-compilers

COPY --from=stage-make /make /usr/local/
# Please note that as per the Dockerfile reference, when the `src` argument is a directory:
# "The directory itself is not copied, just its contents."
# See: https://docs.docker.com/engine/reference/builder/#copy
# Note that this `COPY` does not work with BuildKit.
# See: https://github.com/docker/for-linux/issues/1363
COPY --from=stage-cmake /cmake /usr/local/
COPY --from=stage-inipp /usr/local/include/inipp.h /usr/local/include/
COPY --from=stage-sdl /sdl /usr/local/

RUN \
    useradd --create-home user
