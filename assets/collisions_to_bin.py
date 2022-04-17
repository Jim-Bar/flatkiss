#!/usr/bin/env python3

import os
import re

with open('collisions.bin', 'wb') as collisions_file:
    for file_name in os.listdir():
        if re.match(r'collisions_\d+.txt', file_name):
            with open(file_name) as collisions_text_file:
                collisions = collisions_text_file.readlines()
            
            # Index of the group.
            collisions_file.write(int(re.search(r'\d', file_name)[0]).to_bytes(2, 'little'))
            # Number of collisions in the group.
            collisions_file.write(len(collisions).to_bytes(2, 'little'))
            for collision in [[int(i) for i in collision.split()] for collision in collisions]:
                collisions_file.write(collision[0].to_bytes(2, 'little'))  # Tile index is two bytes.
                for i in collision[1:]:
                    collisions_file.write(i.to_bytes(1, 'little'))  # The rest is one byte (types of collisions, max tile size).
