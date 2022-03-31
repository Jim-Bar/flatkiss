#!/usr/bin/env python3

with open('characters_families.txt') as characters_families_file:
    characters_text = characters_families_file.readlines()

characters = [[int(i) for i in character.split()] for character in characters_text]

with open('characters_families.bin', 'wb') as characters_families_file:
    for character in characters:
        characters_families_file.write(character[0].to_bytes(2, 'little'))
        characters_families_file.write(character[1].to_bytes(1, 'little'))
        characters_families_file.write(character[2].to_bytes(1, 'little'))
