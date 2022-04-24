#!/usr/bin/env python3

import os
import re
import sys

with open('{}.bin'.format(sys.argv[1]), 'wb') as indices_file:
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
