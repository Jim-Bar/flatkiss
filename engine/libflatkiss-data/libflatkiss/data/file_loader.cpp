/*
 * Copyright (C) 2021-2022 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Refer to 'COPYING.txt' for the full notice.
 */

#include <libflatkiss/data/file_loader.hpp>

using std::string;
using std::unordered_map;
using std::vector;

FileLoader::FileLoader(string const& action_sprite_maps_path,
                       string const& animations_path,
                       string const& characters_path, string const& levels_path,
                       string const& solids_path, string const& spritesets_path,
                       string const& tile_solid_maps_path)
    : action_sprite_maps_path_{action_sprite_maps_path},
      animations_path_{animations_path},
      characters_path_{characters_path},
      levels_path_{levels_path},
      solids_path_{solids_path},
      spritesets_path_{spritesets_path},
      tile_solid_maps_path_{tile_solid_maps_path} {}

Model FileLoader::load() const {
  vector<Spriteset> const spritesets{SpritesetLoader::load(spritesets_path_)};
  unordered_map<int64_t, AnimationPlayer const> animation_players{
      AnimationPlayerLoader::load(animations_path_)};
  unordered_map<int64_t, ActionSpriteMapper const> action_sprite_mappers{
      ActionSpriteMapperLoader::load(action_sprite_maps_path_)};
  unordered_map<int64_t, TileSolidMapper const> tile_solid_mappers{
      TileSolidMapperLoader::load(tile_solid_maps_path_)};
  unordered_map<int64_t, Solid const> solids{SolidLoader::load(solids_path_)};

  auto [characters_to_controllers, character_templates]{
      CharacterLoader::load(characters_path_, spritesets, action_sprite_mappers,
                            animation_players, solids)};
  vector<Character> characters;
  vector<Level> const levels{LevelLoader::load(
      levels_path_, spritesets, animation_players, tile_solid_mappers,
      character_templates, characters)};

  return Model{action_sprite_mappers, animation_players, levels, spritesets,
               tile_solid_mappers};
}
