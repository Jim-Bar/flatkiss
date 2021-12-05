#!/usr/bin/env python3

with open('level.txt') as level_file:
    level_text = level_file.read()

tiles = [int(i) for i in level_text.split()]

with open('level.bin', 'wb') as level_file:
    for i in tiles:
        level_file.write((i).to_bytes(1, 'big'))
