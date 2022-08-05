#!/usr/bin/env python3

import math
import struct

with open('level.bin', 'rb') as level_file:
    level_bytes = level_file.read()
tiles = struct.unpack('H' * (len(level_bytes) // 2), level_bytes)

# FIXME: No more work, now the dimensions of the level are directly in the file. Read from that.
width_in_tiles = int(math.sqrt(len(tiles)))
rows = [tiles[i:i + width_in_tiles] for i in range(0, len(tiles), width_in_tiles)]

with open('level.txt', 'w') as level_file:
    level_file.write('\n'.join([' '.join([str(index).zfill(3) for index in row]) for row in rows]))
