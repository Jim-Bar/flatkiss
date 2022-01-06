FROM debian:bullseye-slim

RUN \
    --mount=type=cache,target=/var/cache/apt,sharing=locked --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt update && apt install --yes \
        cmake \
        g++ \
        libsdl2-dev \
    && useradd --create-home user
