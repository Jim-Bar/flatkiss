#!/usr/bin/env python3

import pyglet
import struct

from pyglet.window import key
from pyglet.window import mouse

TILE_SIZE_PIXELS = 16
TILESET_WIDTH_TILES = 24  # FIXME: Deduce from image width and size of tiles.
TILESET_HEIGHT_TILES = 25  # FIXME: Idem
TILESET_OFFSET_PIXELS = 1
TILESET_BOTTOM_OFFSET_PIXELS = 2  # FIXME: Deduce from image height, TILESET_HEIGHT_TILES and TILESET_OFFSET_PIXELS
LEVEL_WIDTH_TILES = 20
LEVEL_HEIGHT_TILES = LEVEL_WIDTH_TILES
LEVEL_SIZE_TILES = LEVEL_WIDTH_TILES * LEVEL_HEIGHT_TILES
SPEED_IN_PIXELS = 3
VIEWPORT_SIZE = 10 * TILE_SIZE_PIXELS


def load_level():
    with open('level.bin', 'rb') as level_file:
        level_bytes = level_file.read()
    # Convert each two bytes to an unsigned int.
    return struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

level = load_level()

tileset = pyglet.image.load('../assets/tileset.bmp')

tileset_window = pyglet.window.Window(caption="Tileset", width=tileset.width, height=tileset.height)
level_window = pyglet.window.Window(caption="Level", width=(LEVEL_WIDTH_TILES * TILE_SIZE_PIXELS), height=(LEVEL_HEIGHT_TILES * TILE_SIZE_PIXELS))


@level_window.event
def on_draw():
    level_window.clear()
    for y in range(0, LEVEL_HEIGHT_TILES):
        for x in range(0, LEVEL_WIDTH_TILES):
            tile_index = level[y * LEVEL_HEIGHT_TILES + x]
            tile = tileset.get_region(
                (tile_index % TILESET_WIDTH_TILES) * (TILE_SIZE_PIXELS + 1) + TILESET_OFFSET_PIXELS,
                (TILESET_HEIGHT_TILES - 1 - tile_index // TILESET_WIDTH_TILES) * (TILE_SIZE_PIXELS + 1) + TILESET_BOTTOM_OFFSET_PIXELS,
                TILE_SIZE_PIXELS,
                TILE_SIZE_PIXELS
            )
            tile.blit(x * TILE_SIZE_PIXELS, level_window.height - (y + 1) * TILE_SIZE_PIXELS)


@level_window.event
def on_close():
    tileset_window.close()


@tileset_window.event
def on_draw():
    tileset_window.clear()
    tileset.blit(0, 0)


@tileset_window.event
def on_close():
    level_window.close()


# Sample for future use.
@tileset_window.event
def on_key_press(symbol, modifiers):
    if symbol == key.A:
        print('The "A" key was pressed.')
    elif symbol == key.LEFT:
        print('The left arrow key was pressed.')
    elif symbol == key.ENTER:
        print('The enter key was pressed.')


# Sample for future use.
@tileset_window.event
def on_mouse_press(x, y, button, modifiers):
    if button == mouse.LEFT:
        print('The left mouse button was pressed.')


pyglet.app.run()
