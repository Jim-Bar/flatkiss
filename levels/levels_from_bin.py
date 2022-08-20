#!/usr/bin/env python3

import math
import os
import struct

with open('levels.bin', 'rb') as level_file:
    level_bytes = level_file.read()

with open('levels.txt', 'w') as level_file:

    while len(level_bytes) > 0:
        width, height, spriteset_index, animation_index, tile_solid_map_index = \
            struct.unpack('HHHHH', level_bytes[:5 * 2])
        tiles = struct.unpack('H' * width * height, level_bytes[5 * 2:5 * 2 + width * height * 2])

        rows = [tiles[i:i + width] for i in range(0, len(tiles), width)]

        level_file.write('{} {} {} {} {}\n'.format(width, height, spriteset_index, animation_index,
                                                   tile_solid_map_index))
        level_file.write('\n'.join([' '.join([str(index).zfill(3) for index in row]) for row in rows]))

        # Prepare for next level.
        level_bytes = level_bytes[5 * 2 + width * height * 2:]

        if len(level_bytes) > 0:
            level_file.write('\n')
