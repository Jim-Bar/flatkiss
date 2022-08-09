#!/usr/bin/env python3

import math
import os
import struct

with open('levels.bin', 'rb') as level_file:
    level_bytes = level_file.read()

if os.path.isfile('levels.txt'):
    os.remove('levels.txt')

while len(level_bytes) > 0:
    width, height, spriteset_index = struct.unpack('HHH', level_bytes[:3 * 2])
    tiles = struct.unpack('H' * width * height, level_bytes[3 * 2:3 * 2 + width * height * 2])

    rows = [tiles[i:i + width] for i in range(0, len(tiles), width)]

    with open('levels.txt', 'a') as level_file:
        level_file.write('{} {} {}\n'.format(width, height, spriteset_index))
        level_file.write('\n'.join([' '.join([str(index).zfill(3) for index in row]) for row in rows]))

        # Prepare for next level.
        if len(level_bytes) > 0:
            level_bytes = level_bytes[3 * 2 + width * height * 2:]
            level_file.write('\n')
