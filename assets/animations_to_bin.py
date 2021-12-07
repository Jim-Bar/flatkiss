#!/usr/bin/env python3

# TODO: Add periods in animations.

with open('animations.txt') as animations_file:
    animations_text = animations_file.readlines()

animations = [[int(i) for i in animation.split()] for animation in animations_text]

with open('animations.bin', 'wb') as animations_file:
    for animation in animations:
        animations_file.write((len(animation) - 1).to_bytes(1, 'little'))  # -1 for removing the duration byte.
        # animations_file.write(bytes(animation))
        animations_file.write(animation[0].to_bytes(1, 'little'))
        for i in animation[1:]:
            animations_file.write(i.to_bytes(2, 'little'))
