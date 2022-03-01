# CMake: https://cmake.org/download
FROM debian:bullseye-slim

ADD https://github.com/Kitware/CMake/releases/download/v3.22.2/cmake-3.22.2-linux-x86_64.tar.gz /cmake.tar.gz

RUN \
    mkdir /cmake \
    && tar -xzf /cmake.tar.gz --directory=/cmake --strip-components=1

# IniPP: https://github.com/mcmtroffaes/inipp
FROM debian:bullseye-slim

ADD https://raw.githubusercontent.com/mcmtroffaes/inipp/46248e4e93a2e63f9a1d0d8d9ad40bd6b3725df5/inipp/inipp.h /usr/local/include/

# Final image
FROM debian:bullseye-slim

# Please note that as per the Dockerfile reference, when the `src` argument is a directory:
# "The directory itself is not copied, just its contents."
# See: https://docs.docker.com/engine/reference/builder/#copy
# Note that this `COPY` does not work with BuildKit.
# See: https://github.com/docker/for-linux/issues/1363
COPY --from=0 /cmake /usr/local/
COPY --from=1 /usr/local/include/inipp.h /usr/local/include

RUN \
    apt update && apt install --yes \
        clang \
        clang-format \
        clang-tidy \
        g++ \
        libsdl2-dev \
    && rm -rf /var/lib/apt/lists/* \
    && useradd --create-home user
