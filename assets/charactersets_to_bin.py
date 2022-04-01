#!/usr/bin/env python3

with open('charactersets.txt') as charactersets_file:
    characters_text = charactersets_file.readlines()

characters = [[int(i) for i in character.split()] for character in characters_text]

with open('charactersets.bin', 'wb') as charactersets_file:
    for character in characters:
        for i in character[:2]:  # Width and height on one byte each.
            charactersets_file.write(i.to_bytes(1, 'little'))
        for i in character[2:]:  # The rest is two bytes each.
            charactersets_file.write(i.to_bytes(2, 'little'))
