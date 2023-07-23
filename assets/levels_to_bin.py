#!/usr/bin/env python3

# Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)

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

with open('levels.txt') as level_file:
    items = level_file.read().split()

with open('levels.bin', 'wb') as level_file:
    while len(items) > 0:
        width = int(items[0])
        height = int(items[1])
        spriteset_index = int(items[2])
        animation_index = int(items[3])
        tile_solid_map_index = int(items[4])
        num_characters = int(items[5])
        # One character is (index, x, y) with each component on two bytes.
        mark = 6 + num_characters * 3
        characters = [int(i) for i in items[6:mark]]
        tiles = [int(i) for i in items[mark:mark + width * height]]

        level_file.write(width.to_bytes(2, 'little'))
        level_file.write(height.to_bytes(2, 'little'))
        level_file.write(spriteset_index.to_bytes(2, 'little'))
        level_file.write(animation_index.to_bytes(2, 'little'))
        level_file.write(tile_solid_map_index.to_bytes(2, 'little'))
        level_file.write(num_characters.to_bytes(2, 'little'))
        for i in characters:
            level_file.write(i.to_bytes(2, 'little'))
        for i in tiles:
            level_file.write(i.to_bytes(2, 'little'))

        items = items[mark + width * height:]
