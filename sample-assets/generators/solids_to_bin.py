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

from typing import DefaultDict, List

import collections
import sys


def solids_to_binary(text_file_path: str, binary_file_path: str) -> None:
    with open(text_file_path) as solids_file:
        lines = solids_file.readlines()

    solid_pieces_per_index: DefaultDict[int, List] = collections.defaultdict(list)

    for line in lines:
        integers = [int(i) for i in line.split()]
        solid_pieces_per_index[integers[0]].append(integers[1::])

    with open(binary_file_path, 'wb') as solids_file:
        for solid_index, solid_pieces in solid_pieces_per_index.items():
            solid_size = len(solid_pieces)
            solids_file.write(solid_index.to_bytes(2, 'little'))
            solids_file.write(solid_size.to_bytes(2, 'little'))
            for solid_piece in solid_pieces:
                for i in solid_piece:
                    solids_file.write(i.to_bytes(1, 'little'))


if __name__ == '__main__':
    solids_to_binary(sys.argv[1], sys.argv[2])
