#!/usr/bin/env python3

with open('levels.txt') as level_file:
    items = level_file.read().split()

width = int(items[0])
height = int(items[1])
spriteset_index = int(items[2])
animation_index = int(items[3])
tile_solid_map_index = int(items[4])
tiles = [int(i) for i in items[5::]]

with open('levels.bin', 'wb') as level_file:
    level_file.write(width.to_bytes(2, 'little'))
    level_file.write(height.to_bytes(2, 'little'))
    level_file.write(spriteset_index.to_bytes(2, 'little'))
    level_file.write(animation_index.to_bytes(2, 'little'))
    level_file.write(tile_solid_map_index.to_bytes(2, 'little'))
    for i in tiles:
        level_file.write(i.to_bytes(2, 'little'))
