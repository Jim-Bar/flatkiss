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

def characters_to_binary(text_file_path: str, binary_file_path: str) -> None:
    with open(text_file_path) as characters_file:
        characters_text = characters_file.readlines()

    characters = [[int(i) for i in character.split()] for character in characters_text]

    with open(binary_file_path, 'wb') as characters_file:
        for character in characters:
            characters_file.write(character[0].to_bytes(2, 'little'))
            characters_file.write(character[1].to_bytes(2, 'little'))
            characters_file.write(character[2].to_bytes(2, 'little'))
            characters_file.write(character[3].to_bytes(2, 'little'))
            characters_file.write(character[4].to_bytes(1, 'little'))


if __name__ == '__main__':
    characters_to_binary('characters.txt', 'characters.bin')
