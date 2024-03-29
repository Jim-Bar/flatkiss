# Docker image containing all the necessary tools for building the project. Using multi-stage builds for keeping the
# image size small. The image defines a default user for Visual Studio Code, preventing it from running as root. When
# running the container manually, just use the host's user. Refer to the `USER` directive below for more information.

# This image is a bit long to build because it installs tools from sources individually. But the focus here is to
# retrieve the most recent versions where possible, and to freeze them (relying on a package manager brings no guarantee
# about that). Some stages could technically be grouped together, but using separation of concerns, each stage is made
# independant from others, reusable and readable.

########################################################################################################################
# Make: https://www.gnu.org/software/make
########################################################################################################################
FROM debian:bullseye-slim AS stage-make

ADD https://ftp.gnu.org/gnu/make/make-4.3.tar.gz /make.tar.gz
WORKDIR /make-build

# The configuration step yields an error without `--disable-dependency-tracking`. Make requires itself to build and
# install. So using `build.sh` which produces a minimal `make` that can then be used to build and install the definitive
# `make` under `/make`. That can be later copied to a location like `/usr/local/`.
RUN \
    apt update \
    && apt install --yes \
        gcc \
    && tar -xzf /make.tar.gz --directory=/make-build --strip-components=1 \
    && mkdir /make \
    && ./configure --disable-dependency-tracking --prefix=/make \
    && ./build.sh \
    && ./make \
    && ./make install

########################################################################################################################
# CMake: https://cmake.org/download
########################################################################################################################
FROM debian:bullseye-slim AS stage-cmake

ADD https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2-linux-x86_64.tar.gz /cmake.tar.gz

RUN \
    mkdir /cmake \
    && tar -xzf /cmake.tar.gz --directory=/cmake --strip-components=1

########################################################################################################################
# IniPP: https://github.com/mcmtroffaes/inipp
########################################################################################################################
FROM debian:bullseye-slim as stage-inipp

ADD https://raw.githubusercontent.com/mcmtroffaes/inipp/46248e4e93a2e63f9a1d0d8d9ad40bd6b3725df5/inipp/inipp.h /usr/local/include/
# Note that `ADD` has `--chmod` in BuildKit.
RUN chmod 644 /usr/local/include/inipp.h

########################################################################################################################
# SDL: https://wiki.libsdl.org/Installation
########################################################################################################################
FROM debian:bullseye-slim AS stage-sdl

ADD https://github.com/libsdl-org/SDL/releases/download/release-2.28.2/SDL2-2.28.2.tar.gz /SDL.tar.gz
WORKDIR /sdl-build

# This installs SDL without worrying about its dependencies. It is likely that it could not run into the container, but
# the important thing is to be able to compile. Running the binary is done outside of the container.
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

########################################################################################################################
# Clang: https://apt.llvm.org
########################################################################################################################
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

########################################################################################################################
# GCC: https://stackoverflow.com/a/61591646
########################################################################################################################
FROM stage-clang AS stage-compilers

# Not using the latest version available (as seen in the link above) but the package maintained one. This because it
# affects other packages installed later, which can lead to conflicts. Morever those are testing packages. Not
# installing from sources either because that is too heavy / long. In addition to that, most recent version of GCC often
# produces a binary relying on a too recent version of GLIBC, making it unusable outside of the container.
RUN \
    apt update \
    && apt install --yes \
        g++ \
    && rm -rf /var/lib/apt/lists/*

########################################################################################################################
# Final image
########################################################################################################################
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

# A user is specifically created for Visual Studio Code, because it defaults to the last `USER` instruction (which is
# root if absent). The UID and GID of this user are not important because Visual Studio Code does some magic with
# `/etc/passwd` for working around permissions problems with bind mounts. See `containerUser`:
# https://code.visualstudio.com/docs/remote/devcontainerjson-reference
# When building directly with the container (without Visual Studio Code) and a bind mount, just start the container with
# the host's user (i.e. `-u $(id -u)`). It does not have to exist in the container, it will work all the same.
RUN \
    useradd --create-home vscode

USER vscode
