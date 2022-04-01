#!/usr/bin/env python3

with open('characters.txt') as characters_file:
    characters_text = characters_file.readlines()

characters = [[int(i) for i in character.split()] for character in characters_text]

with open('characters.bin', 'wb') as characters_file:
    for character in characters:
        characters_file.write(character[0].to_bytes(8, 'little'))
        characters_file.write(character[1].to_bytes(8, 'little'))
        characters_file.write(character[2].to_bytes(2, 'little'))
        characters_file.write(character[3].to_bytes(2, 'little'))
        characters_file.write(character[4].to_bytes(2, 'little'))
        characters_file.write(character[5].to_bytes(1, 'little'))
