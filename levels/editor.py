#!/usr/bin/env python3

import configparser
import pyglet
import struct

from typing import Callable, List

# Important note: pyglet's origin is located in the bottom left of images, unlike SDL which is top left. So for every
# link with the outside of the editor (e.g. when loading), the indices of the tiles are reversed to migrate between the
# two coordinate systems, and the lines the level are inverted as well. Makes it possible to work with the editor's code
# without worrying about reverting things vertically everywhere.
#
# For instance, with a tileset of width and height 3, the indices in both systems are:
#
# SDL   | pyglet
# ------+-------
# 0 1 2 | 6 7 8
# 3 4 5 | 3 4 5
# 6 7 8 | 0 1 2
#
# With the matching level of width and height 4, the lines in both systems (before indices inversion) are:
#
# SDL     | pyglet
# --------+-------
# 4 2 0 7 | 2 1 8 6
# 3 1 0 7 | 8 0 5 3
# 8 0 5 3 | 3 1 0 7
# 2 1 8 6 | 4 2 0 7
#
# And after the indices inversion:
#
# SDL     | pyglet
# --------+-------
# 4 2 0 7 | 8 7 2 0
# 3 1 0 7 | 2 6 5 3
# 8 0 5 3 | 3 7 6 1
# 2 1 8 6 | 4 8 6 1


class _Location(object):
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
        self._current_selected_tile_index = 0
        self._tileset = _Tileset(configuration.tileset_path(), configuration.tiles_size(),
                                 configuration.tileset_width_in_tiles(), configuration.tileset_height_in_tiles(),
                                 configuration.tileset_left_offset(), configuration.tileset_top_offset(),
                                 configuration.tileset_gap())
        self._level = _LevelLoader.load(configuration.level_path(), configuration.level_width_in_tiles(),
                                        configuration.level_height_in_tiles(), self._tileset)
        self._level_window = _LevelWindow(self._level, self._tileset, self.on_window_closed, self.on_save_requested,
                                          self.on_location_selected)
        self._save_path = configuration.level_path()
        self._tileset_window = _TilesetWindow(self._tileset, self.on_window_closed, self.on_save_requested,
                                              self.on_tile_selected)

    def on_location_selected(self, location: _Location) -> None:
        self._level.set_tile_index(location.i, location.j, self._current_selected_tile_index)

    def on_save_requested(self) -> None:
        _LevelLoader.save(self._level, self._save_path, self._tileset)

    def on_tile_selected(self, tile_index: int) -> None:
        self._current_selected_tile_index = tile_index

    def on_window_closed(self) -> None:
        self._tileset_window.close()
        self._level_window.close()

    @staticmethod
    def run() -> None:
        pyglet.app.run()


class _LevelLoader(object):
    """
    Load /save a level from / to disk.
    """

    @staticmethod
    def _reverse_tile_index(tile_index: int, tileset: '_Tileset') -> int:
        """
        Convert a tile index from SDL's (respectively pyglet's) coordinate system (top left) to pyglet's (respectively
        SDL's) coordinate system (bottom left), and vice-versa.

        :param tile_index: index of the tile in one or another of the coordinate systems.
        :param tileset: tileset that will be used with the level.
        :return: index of the tile in the other coordinate system.
        """
        i = tile_index % tileset.width_in_tiles()
        j = tileset.height_in_tiles() - 1 - tile_index // tileset.width_in_tiles()
        return j * tileset.width_in_tiles() + i

    @staticmethod
    def load(path: str, width_in_tiles: int, height_in_tiles: int, tileset: '_Tileset') -> '_Level':
        with open(path, 'rb') as level_file:
            level_bytes = level_file.read()
        # Convert each two bytes to an unsigned int.
        tiles = struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

        # Convert tiles indices in pyglet's coordinate system and invert the rows of the level. Inverting rows is done
        # by cutting the level in lines of length ``level's width``, reverting it, then flattening it.
        tiles = [_LevelLoader._reverse_tile_index(index, tileset)
                 for line in reversed([tiles[i:i + width_in_tiles] for i in range(0, len(tiles), width_in_tiles)])
                 for index in line]

        return _Level(tiles, width_in_tiles, height_in_tiles)

    @staticmethod
    def save(level: '_Level', path: str, tileset: '_Tileset') -> None:
        # Organize the level in rows of tiles, and at the same time convert tiles indices in SDL's coordinate system.
        rows = list()
        for j in range(0, level.height_in_tiles()):
            row = list()
            for i in range(0, level.width_in_tiles()):
                row.append(_LevelLoader._reverse_tile_index(level.tile_index(i, j), tileset))
            rows.append(row)

        # Invert rows of the level and flatten.
        tiles = [tile_index for row in reversed(rows) for tile_index in row]

        with open(path, 'wb') as level_file:
            for i in tiles:
                level_file.write(i.to_bytes(2, 'little'))


class _Level(object):
    """
    Contains a level.
    """

    def __init__(self, tiles: List[int], width_in_tiles: int, height_in_tiles: int) -> None:
        self._width_in_tiles = width_in_tiles
        self._height_in_tiles = height_in_tiles
        self._tiles = tiles

    def height_in_tiles(self) -> int:
        return self._height_in_tiles

    def set_tile_index(self, i: int, j: int, tile_index: int) -> None:
        self._tiles[j * self._width_in_tiles + i] = tile_index

    def tile_index(self, i: int, j: int) -> int:
        return self._tiles[j * self._width_in_tiles + i]

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
            (tile_index // self._width_in_tiles) * (self._tiles_size + self._gap) + self._bottom_offset,
            self._tiles_size,
            self._tiles_size
        )

    def tile_index_at_point_in_tileset(self, point: _Point) -> int:
        i = (point.x - self._left_offset) // (self._tiles_size + self._gap)
        j = (point.y - self._bottom_offset) // (self._tiles_size + self._gap)
        return j * self._width_in_tiles + i

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

    def __init__(self, level: _Level, tileset: _Tileset, on_close: Callable, on_save_requested: Callable,
                 on_location_selected: Callable) -> None:
        self._level = level
        self._on_close_callback = on_close
        self._on_location_selected = on_location_selected
        self._on_save_requested = on_save_requested
        self._origin = _Point(0, 0)
        self._tileset = tileset
        super().__init__(caption='Level', width=319, height=160,
                         resizable=True)
        self.set_maximum_size(level.width_in_tiles() * tileset.tiles_size_in_pixels(),
                              level.height_in_tiles() * tileset.tiles_size_in_pixels())

    def _is_inbounds(self, point: _Point) -> bool:
        return 0 <= point.x < self.width and 0 <= point.y < self.height

    def _level_height_in_pixels(self) -> int:
        return self._level.height_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _level_width_in_pixels(self) -> int:
        return self._level.width_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _tile_location_from_point(self, point: _Point) -> _Location:
        return _Location(point.x // self._tileset.tiles_size_in_pixels(),
                         point.y // self._tileset.tiles_size_in_pixels())

    def _tile_location_from_point_in_window(self, point_in_window: _Point) -> _Location:
        return self._tile_location_from_point(_Point(self._origin.x + point_in_window.x,
                                                     self._origin.y + point_in_window.y))

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        first = self._tile_location_from_point(self._origin)
        last = self._tile_location_from_point(_Point(self._origin.x + self.width - 1,
                                                       self._origin.y + self.height - 1))
        for j in range(first.j, last.j + 1):
            for i in range(first.i, last.i + 1):
                tile_index = self._level.tile_index(i, j)
                tile = self._tileset.tile(tile_index)
                tile.blit(i * self._tileset.tiles_size_in_pixels() - self._origin.x,
                          j * self._tileset.tiles_size_in_pixels() - self._origin.y)

    def on_key_press(self, symbol: int, modifiers: int) -> None:
        if symbol == pyglet.window.key.S:
            self._on_save_requested()

    def on_mouse_drag(self, x: int, y: int, dx: int, dy: int, buttons: int, modifiers: int) -> None:
        if buttons & pyglet.window.mouse.RIGHT != 0:  # Move around using the right mouse button.
            self._origin.x = min(max(self._origin.x - dx, 0), self._level_width_in_pixels() - self.width)
            self._origin.y = min(max(self._origin.y - dy, 0), self._level_height_in_pixels() - self.height)
        elif buttons & pyglet.window.mouse.LEFT != 0:  # Place tiles down with the left mouse button.
            if self._is_inbounds(_Point(x, y)):
                self._on_location_selected(self._tile_location_from_point_in_window(_Point(x, y)))

    def on_mouse_press(self, x: int, y: int, button: int, modifiers: int) -> None:
        if button == pyglet.window.mouse.LEFT:
            self._on_location_selected(self._tile_location_from_point_in_window(_Point(x, y)))

    def on_resize(self, width: int, height: int) -> None:
        super().on_resize(width, height)
        # Avoid going beyond the boundaries of the level when increasing the size of the window.
        self._origin.x = min(self._origin.x, self._level_width_in_pixels() - width)
        self._origin.y = min(self._origin.y, self._level_height_in_pixels() - height)


class _TilesetWindow(pyglet.window.Window):
    """
    Window containing the tileset view.
    """

    def __init__(self, tileset: _Tileset, on_close: Callable, on_save_requested: Callable,
                 on_tile_selected: Callable) -> None:
        self._on_close_callback = on_close
        self._on_save_requested = on_save_requested
        self._on_tile_selected = on_tile_selected
        self._tileset = tileset
        super().__init__(caption='Tileset', width=tileset.width_in_pixels(), height=tileset.height_in_pixels())

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        self._tileset.image().blit(0, 0)

    def on_key_press(self, symbol: int, modifiers: int) -> None:
        if symbol == pyglet.window.key.S:
            self._on_save_requested()

    def on_mouse_press(self, x: int, y: int, button: int, modifiers: int) -> None:
        if button == pyglet.window.mouse.LEFT:
            self._on_tile_selected(self._tileset.tile_index_at_point_in_tileset(_Point(x, y)))


def main():
    _Controller(_Configuration('../configuration.ini')).run()


if __name__ == '__main__':
    main()
