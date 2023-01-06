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

#ifndef LIBFLATKISS_MODEL_ANIMATION_PLAYER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <fstream>
#include <libflatkiss/model/animation.hpp>
#include <unordered_map>

/**
 * @brief Maps sprite indices to their animated counterparts.
 *
 * Given a sprite index and a tick, this class helps determining the index of
 * the sprite which should be displayed.
 */
class AnimationPlayer {
 public:
  /**
   * @brief Construct an AnimationPlayer from a map of animations.
   *
   * @param animations_per_sprite_index Map of sprite indices to their
   * respective animations.
   */
  AnimationPlayer(
      std::unordered_map<uint16_t, Animation>&& animations_per_sprite_index);
  uint16_t animatedSpriteIndexFor(uint16_t sprite_index, int64_t tick) const;
  int64_t animationDurationForSpriteIndex(uint16_t sprite_index) const;

 private:
  std::unordered_map<uint16_t, Animation> const animations_per_sprite_index_;
};

#endif
