#!/usr/bin/env python3

with open('collisions.txt') as collisions_file:
    collisions_text = collisions_file.readlines()

collisions = [[int(i) for i in collision.split()] for collision in collisions_text]

with open('collisions.bin', 'wb') as collisions_file:
    for collision in collisions:
        collisions_file.write(collision[0].to_bytes(2, 'little'))  # Tile index is two bytes.
        for i in collision[1:]:
            collisions_file.write(i.to_bytes(1, 'little'))  # Max tile size is one byte.
