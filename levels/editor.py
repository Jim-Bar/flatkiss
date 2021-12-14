#!/usr/bin/env python3

import configparser
import pyglet
import struct

from typing import Callable

# Important note: pyglet's origin is located in the bottom left of images, unlike SDL which is top left. The reference
# is SDL, so when doing height calculations there are often things like: ``value = height - something``.


class _Coordinate(object):
    """
    Location of a tile (counting in tiles).
    """

    def __init__(self, i: int, j: int) -> None:
        self.i = i
        self.j = j


class _Point(object):
    """
    Location of a pixel (counting in pixels).
    """

    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y


class _Configuration(configparser.ConfigParser):

    _LEVEL = 'Level'
    _TILESET = 'Tileset'

    def __init__(self, path: str) -> None:
        super().__init__()
        self.read(path)

    def level_height_in_tiles(self) -> int:
        return self.getint(_Configuration._LEVEL, 'height_in_tiles')

    def level_path(self) -> str:
        return self.get(_Configuration._LEVEL, 'path')

    def level_width_in_tiles(self) -> int:
        return self.getint(_Configuration._LEVEL, 'width_in_tiles')

    def tiles_size(self) -> int:
        return self.getint(_Configuration._TILESET, 'tiles_size')

    def tileset_gap(self) -> int:
        return self.getint(_Configuration._TILESET, 'gap')

    def tileset_height_in_tiles(self) -> int:
        return self.getint(_Configuration._TILESET, 'height_in_tiles')

    def tileset_left_offset(self) -> int:
        return self.getint(_Configuration._TILESET, 'left_offset')

    def tileset_path(self) -> str:
        return self.get(_Configuration._TILESET, 'path')

    def tileset_top_offset(self) -> int:
        return self.getint(_Configuration._TILESET, 'top_offset')

    def tileset_width_in_tiles(self) -> int:
        return self.getint(_Configuration._TILESET, 'width_in_tiles')


class _Controller(object):
    """
    Windows handling and rendering.
    """

    def __init__(self, configuration: _Configuration) -> None:
        level = _Level(configuration.level_path(), configuration.level_width_in_tiles(),
                       configuration.level_height_in_tiles())
        tileset = _Tileset(configuration.tileset_path(), configuration.tiles_size(),
                           configuration.tileset_width_in_tiles(), configuration.tileset_height_in_tiles(),
                           configuration.tileset_left_offset(), configuration.tileset_top_offset(),
                           configuration.tileset_gap())
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

    def __init__(self, path: str, width_in_tiles: int, height_in_tiles: int) -> None:
        self._width_in_tiles = width_in_tiles
        self._height_in_tiles = height_in_tiles
        with open(path, 'rb') as level_file:
            level_bytes = level_file.read()
        # Convert each two bytes to an unsigned int.
        self._tiles = struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

    def height_in_tiles(self) -> int:
        return self._height_in_tiles

    def tile_index(self, i: int, j: int) -> int:
        return self._tiles[j * self._height_in_tiles + i]

    def width_in_tiles(self) -> int:
        return self._width_in_tiles


class _Tileset(object):
    """
    Wrap a tileset image with useful routines.
    """

    def __init__(self, path: str, tiles_size: int, width_in_tiles: int, height_in_tiles: int, left_offset: int,
                 top_offset: int, gap: int) -> None:
        """
        :param path: path to the image tileset.
        :param tiles_size: width and height of the tiles in pixels.
        :param width_in_tiles: number of tiles making up the tileset horizontally.
        :param height_in_tiles: number of tiles making up the tileset vertically.
        :param left_offset: distance separating the first tile vertically from the left of the image, in pixels.
        :param top_offset: distance separating the first tile vertically from the top of the image, in pixels.
        :param gap: number of pixels separating the tiles.
        """
        self._image = pyglet.image.load(path)
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

    def image(self) -> pyglet.image.AbstractImage:
        return self._image

    def tile(self, tile_index: int) -> pyglet.image.AbstractImage:
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
        self._origin = _Point(0, 0)
        self._tileset = tileset
        super().__init__(caption='Level', width=319, height=160,
                         resizable=True)
        self.set_maximum_size(level.width_in_tiles() * tileset.tiles_size_in_pixels(),
                              level.height_in_tiles() * tileset.tiles_size_in_pixels())

    def _level_height_in_pixels(self) -> int:
        return self._level.height_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _level_width_in_pixels(self) -> int:
        return self._level.width_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _tile_coordinate_from_point(self, point: _Point) -> _Coordinate:
        return _Coordinate(point.x // self._tileset.tiles_size_in_pixels(),
                           point.y // self._tileset.tiles_size_in_pixels())

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        first = self._tile_coordinate_from_point(self._origin)
        last = self._tile_coordinate_from_point(_Point(self._origin.x + self.width - 1,
                                                       self._origin.y + self.height - 1))
        for j in range(first.j, last.j + 1):
            for i in range(first.i, last.i + 1):
                tile_index = self._level.tile_index(i, j)
                tile = self._tileset.tile(tile_index)
                tile.blit(i * self._tileset.tiles_size_in_pixels() - self._origin.x,
                          self.height - (j + 1) * self._tileset.tiles_size_in_pixels() + self._origin.y)

    def on_mouse_drag(self, x: int, y: int, dx: int, dy: int, buttons: int, modifiers: int) -> None:
        # Move around using the right mouse button.
        if buttons & pyglet.window.mouse.RIGHT != 0:
            self._origin.x = min(max(self._origin.x - dx, 0), self._level_width_in_pixels() - self.width)
            self._origin.y = min(max(self._origin.y + dy, 0), self._level_height_in_pixels() - self.height)

    def on_resize(self, width: int, height: int) -> None:
        super().on_resize(width, height)
        # Avoid going beyond the boundaries of the level when increasing the size of the window.
        self._origin.x = min(self._origin.x, self._level_width_in_pixels() - width)
        self._origin.y = min(self._origin.y, self._level_height_in_pixels() - height)


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
    _Controller(_Configuration('../configuration.ini')).run()


if __name__ == '__main__':
    main()
