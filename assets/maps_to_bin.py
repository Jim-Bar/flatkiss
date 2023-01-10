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

import os
import re
import sys

with open('{}s.bin'.format(sys.argv[1]), 'wb') as indices_file:
    for file_name in os.listdir():
        if re.match(r'{}_\d+.txt'.format(sys.argv[1]), file_name):
            with open(file_name) as indices_text_file:
                action_indices_pairs = indices_text_file.readlines()

            # Index of the group.
            indices_file.write(int(re.search(r'\d', file_name)[0]).to_bytes(2, 'little'))
            # Number of action / sprite index couples in the group.
            indices_file.write(len(action_indices_pairs).to_bytes(2, 'little'))

            for action_indices_pair in action_indices_pairs:
                action, sprite_index = action_indices_pair.split()
                indices_file.write(int(action).to_bytes(2, 'little'))
                indices_file.write(int(sprite_index).to_bytes(2, 'little'))
