#!/usr/bin/env python3

# Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

# Refer to 'COPYING.txt' for the full notice.

import animations_to_bin
import characters_to_bin
import contextlib
import fetch_pictures
import levels_to_bin
import maps_to_bin
import pathlib
import solids_to_bin
import spritesets_to_bin


def generate() -> None:
    input_directory = pathlib.Path(__file__).parent.parent / 'assets-as-text'
    output_directory = pathlib.Path.cwd() / 'assets'
    output_directory.mkdir(parents=False, exist_ok=True)

    action_sprite_map = 'action_sprite_map'
    tile_solid_map = 'tile_solid_map'

    # Binary assets.
    animations_to_bin.animations_to_binary(str(input_directory), r'.*animations_\d+.txt',
                                           str(output_directory / 'animations.bin'))
    characters_to_bin.characters_to_binary(str(input_directory / 'characters.txt'),
                                           str(output_directory / 'characters.bin'))
    levels_to_bin.levels_to_binary(str(input_directory / 'levels.txt'),
                                   str(output_directory / 'levels.bin'))
    maps_to_bin.maps_to_binary(str(input_directory), r'.*{}_\d+.txt'.format(action_sprite_map),
                               str(output_directory / '{}s.bin'.format(action_sprite_map)))
    maps_to_bin.maps_to_binary(str(input_directory), r'.*{}_\d+.txt'.format(tile_solid_map),
                               str(output_directory / '{}s.bin'.format(tile_solid_map)))
    solids_to_bin.solids_to_binary(str(input_directory / 'solids.txt'),
                                   str(output_directory / 'solids.bin'))
    spritesets_to_bin.spritesets_to_binary(str(input_directory / 'spritesets.txt'),
                                           str(output_directory / 'spritesets.bin'))

    # Pictures.
    fetch_pictures.fetch_picture('https://www.spriters-resource.com/resources/sheets/9/9445.png?updated=1460953914',
                                 output_directory / 'spriteset_0.png')
    fetch_pictures.fetch_picture('https://i.postimg.cc/8cJk8WKQ/characterset-0.png',
                                 output_directory / 'spriteset_1.png')
    print('WARNING: The pictures must manually be converted to BMP')


if __name__ == '__main__':
    generate()
