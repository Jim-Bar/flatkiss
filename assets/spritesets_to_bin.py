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

with open('spritesets.txt') as spritesets_file:
    characters_text = spritesets_file.readlines()

characters = [[int(i) for i in character.split()] for character in characters_text]

with open('spritesets.bin', 'wb') as spritesets_file:
    for character in characters:
        for i in character[:2]:  # Width and height on one byte each.
            spritesets_file.write(i.to_bytes(1, 'little'))
        for i in character[2:8]:  # Number of sprites and offsets, and texture index.
            spritesets_file.write(i.to_bytes(2, 'little'))
        for i in character[8:11]:  # Colour to turn transparent.
            spritesets_file.write(i.to_bytes(1, 'little'))
