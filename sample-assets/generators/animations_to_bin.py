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

import pathlib
import re
import sys


def animations_to_binary(text_file_path: str, text_file_regex: str, binary_file_path: str) -> None:
    with open(binary_file_path, 'wb') as animations_file:
        for file_path in pathlib.Path(text_file_path).iterdir():
            if re.match(text_file_regex, str(file_path)):
                with open(file_path) as animations_text_file:
                    animations = animations_text_file.readlines()

                # Index of the group.
                animations_file.write(int(re.search(r'\d', file_path.name)[0]).to_bytes(2, 'little'))
                # Number of animations in the group.
                animations_file.write(len(animations).to_bytes(2, 'little'))
                for animation in [[int(i) for i in animation.split()] for animation in animations]:
                    animations_file.write(
                        (len(animation) - 1).to_bytes(1, 'little'))  # -1 for removing the duration byte.
                    animations_file.write(animation[0].to_bytes(1, 'little'))
                    for i in animation[1:]:
                        animations_file.write(i.to_bytes(2, 'little'))


if __name__ == '__main__':
    animations_to_binary(sys.argv[1], sys.argv[2], sys.argv[3])
