#!/usr/bin/env python3

import argparse
import configparser
import pyglet
import struct

from io import FileIO
from typing import Any, Callable, Dict, Generator, List, Tuple

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


def _reverse_tile_index(tile_index: int, tileset_width_in_tiles: int, tileset_height_in_tiles) -> int:
    """
    Convert a tile index from SDL's (respectively pyglet's) coordinate system (top left) to pyglet's (respectively
    SDL's) coordinate system (bottom left), and vice-versa.

    :param tile_index: index of the tile in one or another of the coordinate systems.
    :param tileset_width_in_tiles: width in tiles of the tileset that will be used with the level.
    :param tileset_height_in_tiles: height in tiles of the tileset that will be used with the level.
    :return: index of the tile in the other coordinate system.
    """
    i = tile_index % tileset_width_in_tiles
    j = tileset_height_in_tiles - 1 - tile_index // tileset_width_in_tiles
    return j * tileset_width_in_tiles + i


class _Location(object):
    """
    Location of a tile (counting in tiles) in the level.
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
    Location of a pixel (counting in pixels) in the window.
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


class _AnimationLoader(object):
    """
    Import animations from a file.
    """

    @staticmethod
    def load_from_file(path: str, tileset: '_Tileset', tick_duration_ms: int) -> Dict[int, pyglet.image.Animation]:
        # FIXME: Unhardcode this index.
        target_group_index = 0
        animations = dict()
        current_group_index = target_group_index - 1  # Something not equals to the wanted group index.
        with open(path, 'rb') as animations_file:
            while current_group_index != target_group_index:
                current_group_index, num_animations = struct.unpack('HH', animations_file.read(2 + 2))
                animations = _AnimationLoader._read_group_of_animations(animations_file, num_animations, tileset,
                                                                        tick_duration_ms)

        return animations

    @staticmethod
    def _read_group_of_animations(animations_file: FileIO, num_animations: int, tileset: '_Tileset',
                                  tick_duration_ms: int) -> Dict[int, pyglet.image.Animation]:
        animations = dict()
        for _ in range(num_animations):
            period = animations_file.read(1)[0]
            duration = (tick_duration_ms * animations_file.read(1)[0]) / 1000
            # Convert each two bytes to an unsigned int.
            tiles_indices = struct.unpack('H' * period, animations_file.read(period * 2))
            # Convert tiles indices in pyglet's coordinate system.
            tiles_indices = tuple(_reverse_tile_index(tile_index, tileset.width_in_tiles(), tileset.height_in_tiles())
                                  for tile_index in tiles_indices)
            tiles_images = (tileset.tile(tile_index) for tile_index in tiles_indices)
            animations[tiles_indices[0]] = pyglet.image.Animation.from_image_sequence(tiles_images, duration)

        return animations


class _CommandLineInterface(object):
    """
    Parses the command line.
    """

    def __init__(self) -> None:
        parser = argparse.ArgumentParser(description='Edit levels')
        parser.add_argument('index', type=int, help='the index of the level to edit, starting at zero')
        self._level_index = parser.parse_args().index

    def level_index(self) -> int:
        return self._level_index


class _Configuration(configparser.ConfigParser):

    _ANIMATIONS = 'Animations'
    _EDITOR = 'Editor'
    _ENGINE = 'Engine'
    _LEVELS = 'Levels'
    _SPRITES = 'Sprites'

    def __init__(self, path: str) -> None:
        super().__init__()
        self.read(path)

    def animations_path(self) -> str:
        return self.get(_Configuration._ANIMATIONS, 'path')

    def editor_caption_level_window(self) -> str:
        return self.get(_Configuration._EDITOR, 'caption_level_window')

    def editor_caption_tileset_window(self) -> str:
        return self.get(_Configuration._EDITOR, 'caption_tileset_window')

    def engine_tick_duration_ms(self) -> int:
        return self.getint(_Configuration._ENGINE, 'tick_duration_ms')

    def level_path(self) -> str:
        return self.get(_Configuration._LEVELS, 'path')

    def sprites_files_directory(self) -> str:
        return self.get(_Configuration._SPRITES, 'files_directory')

    def sprites_files_prefix(self) -> str:
        return self.get(_Configuration._SPRITES, 'files_prefix')

    def sprites_files_suffix(self) -> str:
        return self.get(_Configuration._SPRITES, 'files_suffix')

    def sprites_path(self) -> str:
        return self.get(_Configuration._SPRITES, 'path')


class _Controller(object):
    """
    Windows and events handling. Does not manage rendering, this is left for the windows classes.
    """

    def __init__(self, configuration: _Configuration, level_index: int) -> None:
        self._configuration = configuration
        self._level = _LevelLoader.load(configuration.level_path(), level_index)
        self._tileset = _TilesetLoader.load(configuration.sprites_path(), self._level.tileset_index(),
                                            configuration.sprites_files_directory(),
                                            configuration.sprites_files_prefix(), configuration.sprites_files_suffix())
        self._level.reverse(self._tileset)
        # Default to the first index in SDL's coordinate system.
        self._current_selected_tile_index = _reverse_tile_index(0, self._tileset.width_in_tiles(),
                                                                self._tileset.height_in_tiles())
        self._level_window, self._tileset_window = self._create_windows(configuration, self._level, self._tileset)
        self._update_windows_captions(True)

    def _create_windows(self, configuration: _Configuration, level: '_Level',
                        tileset: '_Tileset') -> Tuple['_LevelWindow', '_TilesetWindow']:
        # The tileset window always matches the tileset because it cannot be resized. Then the level window occupies the
        # width left. The height is not conditioned to the tileset because monitors are (almost) always larger than
        # higher, thus the windows are put next to each other by the system instead of next to one another.

        screen = pyglet.canvas.get_display().get_default_screen()

        animations = _AnimationLoader.load_from_file(configuration.animations_path(), tileset,
                                                     configuration.engine_tick_duration_ms())

        level_window_width = min(level.width_in_tiles() * tileset.tiles_size_in_pixels(),
                                 screen.width - tileset.width_in_pixels())
        level_window_height = min(level.height_in_tiles() * tileset.tiles_size_in_pixels(), screen.height)

        level_window = _LevelWindow(level_window_width, level_window_height, level, tileset, animations,
                                    self.on_window_closed, self.on_save_requested, self.on_location_selected)
        tileset_window = _TilesetWindow(tileset.width_in_pixels(), tileset.height_in_pixels(), tileset,
                                        self.on_window_closed, self.on_save_requested, self.on_tile_selected)

        return level_window, tileset_window

    def _update_windows_captions(self, saved: bool) -> None:
        # Display the tile index in SDL's coordinate system.
        reversed_tile_index = _reverse_tile_index(self._current_selected_tile_index, self._tileset.width_in_tiles(),
                                                  self._tileset.height_in_tiles())
        suffix = '({}){}'.format(reversed_tile_index, '' if saved else '*')
        self._level_window.set_caption('{} {}'.format(self._configuration.editor_caption_level_window(), suffix))
        self._tileset_window.set_caption('{} {}'.format(self._configuration.editor_caption_tileset_window(), suffix))

    def on_location_selected(self, location: _Location) -> None:
        self._update_windows_captions(False)
        self._level.set_tile_index(location.i, location.j, self._current_selected_tile_index)

    def on_save_requested(self) -> None:
        _LevelLoader.save(self._level, self._configuration.level_path(), self._tileset)
        self._update_windows_captions(True)

    def on_tile_selected(self, tile_index: int) -> None:
        self._current_selected_tile_index = tile_index
        self._update_windows_captions(False)

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
    def load(path: str, level_index: int) -> '_Level':
        with open(path, 'rb') as level_file:
            return _LevelLoader._read_level_with_index(level_index, level_file)

    @staticmethod
    def _read_level_with_index(level_index: int, level_file: FileIO) -> '_Level':
        # The width and height are in tiles.
        tiles = tuple()
        width = 0
        height = 0
        tileset_index = 0

        # Read all the levels until the right one.
        for _ in range(level_index + 1):
            # Level header length in bytes.
            header = 3 * 2

            # Convert each two bytes to an unsigned int.
            width, height, tileset_index = struct.unpack('HHH', level_file.read(header))
            length = width * height * 2
            tiles = list(struct.unpack('H' * width * height, level_file.read(length)))

        return _Level(level_index, tiles, width, height, tileset_index)

    @staticmethod
    def save(level: '_Level', path: str, tileset: '_Tileset') -> None:
        # Convert tiles indices in SDL's coordinate system.
        level.reverse(tileset)

        with open(path, 'rb+') as level_file:
            if level.index() > 0:
                # Move to the right location in the file where to write the level.
                _LevelLoader._read_level_with_index(level.index() - 1, level_file)
            level_file.write(level.width_in_tiles().to_bytes(2, 'little'))
            level_file.write(level.height_in_tiles().to_bytes(2, 'little'))
            level_file.write(level.tileset_index().to_bytes(2, 'little'))

            for i in level.tiles_generator():
                level_file.write(i.to_bytes(2, 'little'))

        # Convert tiles indices back to pyglet's coordinate system.
        level.reverse(tileset)


class _Level(object):
    """
    Model for a level.
    """

    def __init__(self, index: int, tiles: List[int], width_in_tiles: int, height_in_tiles: int,
                 tileset_index: int) -> None:
        self._index = index
        self._tiles = tiles
        self._width_in_tiles = width_in_tiles
        self._height_in_tiles = height_in_tiles
        self._tileset_index = tileset_index

    def height_in_tiles(self) -> int:
        return self._height_in_tiles

    def index(self) -> int:
        return self._index

    def reverse(self, tileset: '_Tileset') -> None:
        # Convert tiles indices from SDL's coordinate system to pyglet's coordinate system or vice-versa, and invert the
        # rows of the level. Inverting rows is done by cutting the level in lines of length ``level's width``, reverting
        # it, then flattening it.
        self._tiles = [_reverse_tile_index(index, tileset.width_in_tiles(), tileset.height_in_tiles())
                       for line in reversed([self._tiles[i:i + self._width_in_tiles]
                                             for i in range(0, len(self._tiles), self._height_in_tiles)])
                       for index in line]

    def set_tile_index(self, i: int, j: int, tile_index: int) -> None:
        self._tiles[j * self._width_in_tiles + i] = tile_index

    def tiles_generator(self) -> Generator[int, None, None]:
        for tile_index in self._tiles:
            yield tile_index

    def tileset_index(self) -> int:
        return self._tileset_index

    def tile_index(self, i: int, j: int) -> int:
        return self._tiles[j * self._width_in_tiles + i]

    def width_in_tiles(self) -> int:
        return self._width_in_tiles


class _TilesetLoader(object):
    """
    Load a tileset from disk.
    """

    @staticmethod
    def load(path: str, tileset_index: int, files_directory: str, files_prefix: str, files_suffix: str) -> '_Tileset':
        with open(path, 'rb') as tileset_file:
            # Tileset description length in bytes
            tileset_length = 1 + 1 + 2 + 2 + 2 + 2 + 2 + 2 + 1 + 1 + 1

            tileset = None
            for _ in range(tileset_index + 1):
                tiles_width, _, width_in_tiles, height_in_tiles, left_offset, top_offset, gap, picture_file, _, _, _ = \
                    struct.unpack('BBHHHHHHBBB', tileset_file.read(tileset_length))

                tileset = _Tileset('{}/{}{}{}'.format(files_directory, files_prefix, picture_file, files_suffix),
                                   tiles_width, width_in_tiles, height_in_tiles, left_offset, top_offset, gap)

            return tileset


class _Tileset(object):
    """
    Model for a tileset. Wraps a tileset image with useful routines.
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
    Window containing the level view. Handles all the rendering related to the level view. Events handling is delegated
    to the :class:`_Controller`.
    """

    def __init__(self, width: int, height: int, level: _Level, tileset: _Tileset,
                 animations: Dict[int, pyglet.image.Animation], on_close: Callable, on_save_requested: Callable,
                 on_location_selected: Callable) -> None:
        self._animations = animations
        self._batch = pyglet.graphics.Batch()
        self._level = level
        self._on_close_callback = on_close
        self._on_location_selected = on_location_selected
        self._on_save_requested = on_save_requested
        self._origin = _Point(0, 0)
        self._tileset = tileset
        self._tiles = self._create_tiles_sprites()
        super().__init__(width=width, height=height, resizable=True)
        self.set_maximum_size(self._level_width_in_pixels(), self._level_height_in_pixels())

    def _build_tile_sprite_at_location(self, location: _Location) -> pyglet.sprite.Sprite:
        point = self._point_in_window_from_location(location)
        tile_index = self._level.tile_index(location.i, location.j)
        if tile_index in self._animations:
            tile_image = self._animations[tile_index]
        else:
            tile_image = self._tileset.tile(tile_index)

        return pyglet.sprite.Sprite(tile_image, point.x, point.y, batch=self._batch)

    def _create_tiles_sprites(self) -> List[List[pyglet.sprite.Sprite]]:
        tiles = list()
        for i in range(0, self._level.width_in_tiles()):
            tiles.append(list())
            for j in range(0, self._level.height_in_tiles()):
                tiles[i].append(self._build_tile_sprite_at_location(_Location(i, j)))

        return tiles

    def _is_inbounds(self, point: _Point) -> bool:
        return 0 <= point.x < self.width and 0 <= point.y < self.height

    def _level_height_in_pixels(self) -> int:
        return self._level.height_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _level_width_in_pixels(self) -> int:
        return self._level.width_in_tiles() * self._tileset.tiles_size_in_pixels()

    def _point_in_window_from_location(self, location: _Location) -> _Point:
        return _Point(location.i * self._tileset.tiles_size_in_pixels() - self._origin.x,
                      location.j * self._tileset.tiles_size_in_pixels() - self._origin.y)

    def _tile_location_from_point(self, point: _Point) -> _Location:
        return _Location(point.x // self._tileset.tiles_size_in_pixels(),
                         point.y // self._tileset.tiles_size_in_pixels())

    def _tile_location_from_point_in_window(self, point_in_window: _Point) -> _Location:
        return self._tile_location_from_point(_Point(self._origin.x + point_in_window.x,
                                                     self._origin.y + point_in_window.y))

    def _update_tile_at_location(self, location: _Location) -> None:
        self._tiles[location.i][location.j].delete()
        self._tiles[location.i][location.j] = self._build_tile_sprite_at_location(location)

    def _update_tiles_positions(self) -> None:
        for i in range(0, self._level.width_in_tiles()):
            for j in range(0, self._level.height_in_tiles()):
                point = self._point_in_window_from_location(_Location(i, j))
                self._tiles[i][j].update(point.x, point.y)

    def on_close(self) -> None:
        self._on_close_callback()

    def on_draw(self) -> None:
        self.clear()
        self._batch.draw()

    def on_key_press(self, symbol: int, modifiers: int) -> None:
        if symbol == pyglet.window.key.S:
            self._on_save_requested()

    def on_mouse_drag(self, x: int, y: int, dx: int, dy: int, buttons: int, modifiers: int) -> None:
        if buttons & pyglet.window.mouse.RIGHT != 0:  # Move around using the right mouse button.
            self._origin.x = min(max(self._origin.x - dx, 0), self._level_width_in_pixels() - self.width)
            self._origin.y = min(max(self._origin.y - dy, 0), self._level_height_in_pixels() - self.height)
            self._update_tiles_positions()
        elif buttons & pyglet.window.mouse.LEFT != 0:  # Place tiles down with the left mouse button.
            cursor_point = _Point(x, y)
            if self._is_inbounds(cursor_point):
                tile_location = self._tile_location_from_point_in_window(cursor_point)
                self._on_location_selected(tile_location)
                self._update_tile_at_location(tile_location)

    def on_mouse_press(self, x: int, y: int, button: int, modifiers: int) -> None:
        if button == pyglet.window.mouse.LEFT:
            cursor_point = _Point(x, y)
            tile_location = self._tile_location_from_point_in_window(cursor_point)
            self._on_location_selected(tile_location)
            self._update_tile_at_location(tile_location)

    def on_resize(self, width: int, height: int) -> None:
        super().on_resize(width, height)
        # Avoid going beyond the boundaries of the level when increasing the size of the window.
        self._origin.x = min(self._origin.x, self._level_width_in_pixels() - width)
        self._origin.y = min(self._origin.y, self._level_height_in_pixels() - height)
        self._update_tiles_positions()


class _TilesetWindow(pyglet.window.Window):
    """
    Window containing the tileset view. Handles all the rendering related to the tileset view. Events handling is
    delegated to the :class:`_Controller`.
    """

    def __init__(self, width: int, height: int, tileset: _Tileset, on_close: Callable, on_save_requested: Callable,
                 on_tile_selected: Callable) -> None:
        self._on_close_callback = on_close
        self._on_save_requested = on_save_requested
        self._on_tile_selected = on_tile_selected
        self._tileset = tileset
        super().__init__(width=width, height=height)

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
    _Controller(_Configuration('configuration.ini'), _CommandLineInterface().level_index()).run()


if __name__ == '__main__':
    main()
