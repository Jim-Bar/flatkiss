#!/usr/bin/env python3

import os
import re

with open('animations.bin', 'wb') as animations_file:
    for file_name in os.listdir():
        if re.match(r'animations_\d+.txt', file_name):
            with open(file_name) as animations_text_file:
                animations = animations_text_file.readlines()

            # Index of the group.
            animations_file.write(int(re.search(r'\d', file_name)[0]).to_bytes(2, 'little'))
            # Number of animations in the group.
            animations_file.write(len(animations).to_bytes(2, 'little'))
            for animation in [[int(i) for i in animation.split()] for animation in animations]:
                animations_file.write((len(animation) - 1).to_bytes(1, 'little'))  # -1 for removing the duration byte.
                animations_file.write(animation[0].to_bytes(1, 'little'))
                for i in animation[1:]:
                    animations_file.write(i.to_bytes(2, 'little'))
