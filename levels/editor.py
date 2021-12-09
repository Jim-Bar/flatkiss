#!/usr/bin/env python3

import pyglet
import struct

from pyglet.image import AbstractImage
from typing import Callable

# Important note: pyglet's origin is located in the bottom left of images, unlike SDL which is top left. The reference
# is SDL, so when doing height calculations there are often things like: ``value = height - something``.


class _Controller(object):
    """
    Windows handling and rendering.
    """

    def __init__(self) -> None:
        level = _Level('level.bin', 20, 20)
        tileset = _Tileset('../assets/tileset.bmp', 16, 24, 25, 1, 1, 1)
        self._level_window = _LevelWindow(level, tileset, self.on_window_closed)
        self._tileset_window = _TilesetWindow(tileset, self.on_window_closed)

    @staticmethod
    def run() -> None:
        pyglet.app.run()

    def on_window_closed(self) -> None:
        self._tileset_window.close()
        self._level_window.close()


class _Level(object):
    """
    Contains a level.
    """

    def __init__(self, level_path: str, width_in_tiles: int, height_in_tiles: int) -> None:
        self._width_in_tiles = width_in_tiles
        self._height_in_tiles = height_in_tiles
        with open(level_path, 'rb') as level_file:
            level_bytes = level_file.read()
        # Convert each two bytes to an unsigned int.
        self._tiles = struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

    def height_in_tiles(self) -> int:
        return self._height_in_tiles

    def tile_index(self, x: int, y: int) -> int:
        return self._tiles[y * self._height_in_tiles + x]

    def width_in_tiles(self) -> int:
        return self._width_in_tiles


class _Tileset(object):
    """
    Wrap a tileset image with useful routines.
    """

    def __init__(self, image_path: str, tiles_size: int, width_in_tiles: int, height_in_tiles: int, left_offset: int,
                 top_offset: int, gap: int) -> None:
        """
        :param image_path: path to the image tileset.
        :param tiles_size: width and height of the tiles in pixels.
        :param width_in_tiles: number of tiles making up the tileset horizontally.
        :param height_in_tiles: number of tiles making up the tileset vertically.
        :param left_offset: distance separating the first tile vertically from the left of the image, in pixels.
        :param top_offset: distance separating the first tile vertically from the top of the image, in pixels.
        :param gap: number of pixels separating the tiles.
        """
        self._image = pyglet.image.load(image_path)
        self._tiles_size = tiles_size
        self._width_in_tiles = width_in_tiles
        self._height_in_tiles = height_in_tiles
        self._left_offset = left_offset
        # ``+ gap`` because the bottom gap of the last tile is counted in the bottom offset.
        self._bottom_offset = self._image.height - height_in_tiles * (tiles_size + gap) - top_offset + gap
        self._gap = gap

    def height_in_tiles(self) -> int:
        return self._height_in_tiles

    def height_in_pixels(self) -> int:
        return self._image.height

    def image(self) -> AbstractImage:
        return self._image

    def tile(self, tile_index: int) -> AbstractImage:
        return self._image.get_region(
            (tile_index % self._width_in_tiles) * (self._tiles_size + self._gap) + self._left_offset,
            (self._height_in_tiles - 1 - tile_index // self._width_in_tiles) * (self._tiles_size + self._gap)
            + self._bottom_offset,
            self._tiles_size,
            self._tiles_size
        )

    def tiles_size_in_pixels(self) -> int:
        return self._tiles_size

    def width_in_tiles(self) -> int:
        return self._width_in_tiles

    def width_in_pixels(self) -> int:
        return self._image.width


class _LevelWindow(pyglet.window.Window):
    """
    Window containing the level view.
    """

    def __init__(self, level: _Level, tileset: _Tileset, on_close: Callable) -> None:
        self._level = level
        self._on_close_callback = on_close
        self._tileset = tileset
        super().__init__(caption='Level', width=(level.width_in_tiles() * tileset.tiles_size_in_pixels()),
                         height=(level.height_in_tiles() * tileset.tiles_size_in_pixels()))

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        for y in range(0, self._level.height_in_tiles()):
            for x in range(0, self._level.width_in_tiles()):
                tile_index = self._level.tile_index(x, y)
                tile = self._tileset.tile(tile_index)
                tile.blit(x * self._tileset.tiles_size_in_pixels(),
                          self.height - (y + 1) * self._tileset.tiles_size_in_pixels())


class _TilesetWindow(pyglet.window.Window):
    """
    Window containing the tileset view.
    """

    def __init__(self, tileset: _Tileset, on_close: Callable) -> None:
        self._on_close_callback = on_close
        self._tileset = tileset
        super().__init__(caption='Tileset', width=tileset.width_in_pixels(), height=tileset.height_in_pixels())

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        self._tileset.image().blit(0, 0)


def main():
    _Controller().run()


if __name__ == '__main__':
    main()
