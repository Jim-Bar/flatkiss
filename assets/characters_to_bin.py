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

with open('characters.txt') as characters_file:
    characters_text = characters_file.readlines()

characters = [[int(i) for i in character.split()] for character in characters_text]

with open('characters.bin', 'wb') as characters_file:
    for character in characters:
        characters_file.write(character[0].to_bytes(8, 'little'))
        characters_file.write(character[1].to_bytes(8, 'little'))
        characters_file.write(character[2].to_bytes(2, 'little'))
        characters_file.write(character[3].to_bytes(2, 'little'))
        characters_file.write(character[4].to_bytes(2, 'little'))
        characters_file.write(character[5].to_bytes(2, 'little'))
        characters_file.write(character[6].to_bytes(1, 'little'))
