#!/usr/bin/env bash

# This script is temporary. It is used for setting up the assets, building the
# docker image and setting the correct Git user. But in the future:
# - the assets will be downloaded from an artifact server
# - the Docker image will be pulled from a registry
# - no solution yet for the Git user, but that will have to be handled

set -e

git config user.name "Jean-Marie Baran"
git config user.email "jeanmarie.baran@gmail.com"

cd levels
python3 levels_to_bin.py
cd ..

cd assets
python3 animations_to_bin.py
python3 spritesets_to_bin.py
python3 characters_to_bin.py
python3 maps_to_bin.py action_sprite_map
python3 maps_to_bin.py tile_solid_map
python3 solids_to_bin.py
wget --output-document spriteset_0.png "https://www.spriters-resource.com/resources/sheets/9/9445.png?updated=1460953914"
wget --output-document spriteset_1.png "https://i.postimg.cc/8cJk8WKQ/characterset-0.png"
cd ..

#docker build -t flatkiss .
