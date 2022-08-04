#!/usr/bin/env python3

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
