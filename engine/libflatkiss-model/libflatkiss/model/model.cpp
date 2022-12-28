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

#include <libflatkiss/model/model.hpp>

using std::unordered_map;
using std::vector;

Model::Model(
    unordered_map<int64_t, ActionSpriteMapper const> const
        action_sprite_mappers,
    unordered_map<int64_t, AnimationPlayer const> const animation_players,
    vector<Level> const levels, vector<Spriteset> const spritesets,
    unordered_map<int64_t, TileSolidMapper const> const tile_solid_mappers)
    : action_sprite_mappers_{action_sprite_mappers},
      animation_players_{animation_players},
      levels_{levels},
      spritesets_{spritesets},
      tile_solid_mappers_{tile_solid_mappers} {}

unordered_map<int64_t, ActionSpriteMapper const>
Model::action_sprite_mappers() const {
  return action_sprite_mappers_;
}

unordered_map<int64_t, AnimationPlayer const> Model::animation_players()
    const {
  return animation_players_;
}

vector<Level> Model::levels() const { return levels_; }

vector<Spriteset> Model::spritesets() const { return spritesets_; }

unordered_map<int64_t, TileSolidMapper const> Model::tile_solid_mappers()
    const {
  return tile_solid_mappers_;
}
