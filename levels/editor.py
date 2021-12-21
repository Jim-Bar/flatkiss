#!/usr/bin/env python3

import configparser
import pyglet
import struct

from typing import Any, Callable, Dict, List, Tuple

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


class _Location(object):
    """
    Location of a tile (counting in tiles).
    """

    def __init__(self, i: int, j: int) -> None:
        self.i = i
        self.j = j

    def __eq__(self, other: Any) -> bool:
        if isinstance(other, _Location):
            return self.i == other.i and self.j == other.j
        return NotImplemented

    def __hash__(self) -> int:
        return hash(tuple(sorted(self.__dict__.items())))


class _Point(object):
    """
    Location of a pixel (counting in pixels).
    """

    def __init__(self, x: int, y: int) -> None:
        self.x = x
        self.y = y

    def __eq__(self, other: Any) -> bool:
        if isinstance(other, _Point):
            return self.x == other.x and self.y == other.y
        return NotImplemented

    def __hash__(self) -> int:
        return hash(tuple(sorted(self.__dict__.items())))


class _Animation(object):
    """
    Class representing a animation for a particular tile.
    """

    def __init__(self, duration: int, tiles_indices: Tuple[int]) -> None:
        self._duration = duration
        self._tiles_indices = tiles_indices

    def duration(self) -> int:
        return self._duration

    def period(self) -> int:
        return len(self._tiles_indices)

    def tile_index_at_step(self, step: int) -> int:
        return self._tiles_indices[step]


class _AnimationLoader(object):
    """
    Import animations from a file.
    """

    @staticmethod
    def load_from_file(path: str, tileset: '_Tileset') -> Dict[int, _Animation]:
        animations = dict()
        with open(path, 'rb') as animations_file:
            byte_array = animations_file.read(1)
            while len(byte_array) > 0:
                period = byte_array[0]
                duration = animations_file.read(1)[0]
                # Convert each two bytes to an unsigned int.
                tiles_indices = struct.unpack('H' * period, animations_file.read(period * 2))
                # Convert tiles indices in pyglet's coordinate system.
                tiles_indices = tuple(_reverse_tile_index(tile_index, tileset) for tile_index in tiles_indices)
                animations[tiles_indices[0]] = _Animation(duration, tiles_indices)
                byte_array = animations_file.read(1)

        return animations


class _AnimationPlayer(object):
    """
    Maps the current tile and tick to the according tile to show.
    """

    def __init__(self, animations: Dict[int, _Animation]) -> None:
        self._animations = animations

    def animated_tile_index_for(self, tile_index: int, tick: int) -> int:
        if tile_index in self._animations:
            animation = self._animations[tile_index]
            return animation.tile_index_at_step((tick % (animation.period() * animation.duration()))
                                                // animation.duration())

        return tile_index


class _Configuration(configparser.ConfigParser):

    _ANIMATIONS = 'Animations'
    _EDITOR = 'Editor'
    _LEVEL = 'Level'
    _TILESET = 'Tileset'

    def __init__(self, path: str) -> None:
        super().__init__()
        self.read(path)

    def animations_path(self) -> str:
        return self.get(_Configuration._ANIMATIONS, 'path')

    def editor_caption_level_window(self) -> str:
        return self.get(_Configuration._EDITOR, 'caption_level_window')

    def editor_caption_tileset_window(self) -> str:
        return self.get(_Configuration._EDITOR, 'caption_tileset_window')

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
        self._configuration = configuration
        self._current_selected_tile_index = 0
        self._tileset = _Tileset(configuration.tileset_path(), configuration.tiles_size(),
                                 configuration.tileset_width_in_tiles(), configuration.tileset_height_in_tiles(),
                                 configuration.tileset_left_offset(), configuration.tileset_top_offset(),
                                 configuration.tileset_gap())
        self._level = _LevelLoader.load(configuration.level_path(), configuration.level_width_in_tiles(),
                                        configuration.level_height_in_tiles(), self._tileset)
        self._level_window, self._tileset_window = self._create_windows(configuration, self._level, self._tileset)

    def _create_windows(self, configuration: _Configuration, level: '_Level',
                        tileset: '_Tileset') -> Tuple['_LevelWindow', '_TilesetWindow']:
        # The tileset window always matches the tileset because it cannot be resized. Then the level window occupies the
        # width left. The height is not conditioned to the tileset because monitors are (almost) always larger than
        # higher, thus the windows are put next to each other by the system instead of next to one another.

        screen = pyglet.canvas.get_display().get_default_screen()

        animation_player = _AnimationPlayer(_AnimationLoader.load_from_file(configuration.animations_path(), tileset))

        level_window_width = min(level.width_in_tiles() * tileset.tiles_size_in_pixels(),
                                 screen.width - tileset.width_in_pixels())
        level_window_height = min(level.height_in_tiles() * tileset.tiles_size_in_pixels(), screen.height)

        level_window = _LevelWindow(configuration.editor_caption_level_window(), level_window_width,
                                    level_window_height, level, tileset, animation_player, self.on_window_closed,
                                    self.on_save_requested, self.on_location_selected)
        tileset_window = _TilesetWindow(configuration.editor_caption_tileset_window(), tileset.width_in_pixels(),
                                        tileset.height_in_pixels(), tileset, self.on_window_closed,
                                        self.on_save_requested, self.on_tile_selected)

        return level_window, tileset_window

    def on_location_selected(self, location: _Location) -> None:
        self._level_window.set_caption('{}*'.format(self._configuration.editor_caption_level_window()))
        self._tileset_window.set_caption('{}*'.format(self._configuration.editor_caption_tileset_window()))
        self._level.set_tile_index(location.i, location.j, self._current_selected_tile_index)

    def on_save_requested(self) -> None:
        _LevelLoader.save(self._level, self._configuration.level_path(), self._tileset)
        self._level_window.set_caption(self._configuration.editor_caption_level_window())
        self._tileset_window.set_caption(self._configuration.editor_caption_tileset_window())

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
    Load / save a level from / to disk.
    """

    @staticmethod
    def load(path: str, width_in_tiles: int, height_in_tiles: int, tileset: '_Tileset') -> '_Level':
        with open(path, 'rb') as level_file:
            level_bytes = level_file.read()
        # Convert each two bytes to an unsigned int.
        tiles = struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

        # Convert tiles indices in pyglet's coordinate system and invert the rows of the level. Inverting rows is done
        # by cutting the level in lines of length ``level's width``, reverting it, then flattening it.
        tiles = [_reverse_tile_index(index, tileset)
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
                row.append(_reverse_tile_index(level.tile_index(i, j), tileset))
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

    def __init__(self, caption: str, width: int, height: int, level: _Level, tileset: _Tileset,
                 animation_player: _AnimationPlayer, on_close: Callable, on_save_requested: Callable,
                 on_location_selected: Callable) -> None:
        self._animation_player = animation_player
        self._level = level
        self._on_close_callback = on_close
        self._on_location_selected = on_location_selected
        self._on_save_requested = on_save_requested
        self._origin = _Point(0, 0)
        self._tileset = tileset
        self._ticks = 0
        super().__init__(caption=caption, width=width, height=height, resizable=True)
        self.set_maximum_size(level.width_in_tiles() * tileset.tiles_size_in_pixels(),
                              level.height_in_tiles() * tileset.tiles_size_in_pixels())
        pyglet.clock.schedule_interval(self._next_tick, 1 / 60)

    def _next_tick(self, _) -> None:
        self._ticks += 1

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
        last = self._tile_location_from_point(_Point(self._origin.x + self.width - 1, self._origin.y + self.height - 1))
        for j in range(first.j, last.j + 1):
            for i in range(first.i, last.i + 1):
                tile_index = self._animation_player.animated_tile_index_for(self._level.tile_index(i, j), self._ticks)
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
            cursor_point = _Point(x, y)
            if self._is_inbounds(cursor_point):
                self._on_location_selected(self._tile_location_from_point_in_window(cursor_point))

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

    def __init__(self, caption: str, width: int, height: int, tileset: _Tileset, on_close: Callable,
                 on_save_requested: Callable, on_tile_selected: Callable) -> None:
        self._on_close_callback = on_close
        self._on_save_requested = on_save_requested
        self._on_tile_selected = on_tile_selected
        self._tileset = tileset
        super().__init__(caption=caption, width=width, height=height)

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
