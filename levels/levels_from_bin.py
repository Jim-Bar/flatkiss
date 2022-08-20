#!/usr/bin/env python3

# Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Refer to 'COPYING.txt' for the full notice.

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
