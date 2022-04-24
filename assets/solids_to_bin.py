#!/usr/bin/env python3

import collections

with open('solids.txt') as solids_file:
    lines = solids_file.readlines()

solid_pieces_per_index = collections.defaultdict(list)

for line in lines:
    integers = [int(i) for i in line.split()]
    solid_pieces_per_index[integers[0]].append(integers[1::])

with open('solids.bin', 'wb') as solids_file:
    for solid_index, solid_pieces in solid_pieces_per_index.items():
        solid_size = len(solid_pieces)
        solids_file.write(solid_index.to_bytes(2, 'little'))
        solids_file.write(solid_size.to_bytes(2, 'little'))
        for solid_piece in solid_pieces:
            for i in solid_piece:
                solids_file.write(i.to_bytes(1, 'little'))
