/*
 * Copyright (C) 2021-2023 Jean-Marie BARAN (jeanmarie.baran@gmail.com)
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

#ifndef LIBFLATKISS_MODEL_CHARACTER_TEMPLATE_HPP_INCLUDED
#define LIBFLATKISS_MODEL_CHARACTER_TEMPLATE_HPP_INCLUDED

#include <libflatkiss/model/action_sprite_mapper.hpp>
#include <libflatkiss/model/animation_player.hpp>
#include <libflatkiss/model/controller_type.hpp>
#include <libflatkiss/model/solid.hpp>
#include <libflatkiss/model/spriteset.hpp>
#include <vector>

class CharacterTemplate {
 public:
  CharacterTemplate(ActionSpriteMapper const& action_sprite_mapper,
                    AnimationPlayer const& animation_player,
                    std::vector<ControllerType> const& controllers,
                    Spriteset const& spriteset, Solid const& solid);
  ActionSpriteMapper const& action_sprite_mapper() const;
  AnimationPlayer const& animation_player() const;
  std::vector<ControllerType> const& controllers() const;
  Spriteset const& spriteset() const;
  Solid solid() const;

 private:
  ActionSpriteMapper const& action_sprite_mapper_;
  AnimationPlayer const& animation_player_;
  std::vector<ControllerType> const controllers_;
  Spriteset const& spriteset_;
  Solid const solid_;
};

#endif
