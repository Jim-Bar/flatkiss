FROM debian:bullseye-slim

RUN \
    --mount=type=cache,target=/var/cache/apt,sharing=locked --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt update && apt install --yes \
        wget \
    && wget --directory-prefix /usr/local/include https://raw.githubusercontent.com/mcmtroffaes/inipp/46248e4e93a2e63f9a1d0d8d9ad40bd6b3725df5/inipp/inipp.h

FROM debian:bullseye-slim

COPY --from=0 /usr/local/include/inipp.h /usr/local/include

RUN \
    --mount=type=cache,target=/var/cache/apt,sharing=locked --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt update && apt install --yes \
        clang \
        clang-format \
        clang-tidy \
        cmake \
        g++ \
        libsdl2-dev \
    && useradd --create-home user
