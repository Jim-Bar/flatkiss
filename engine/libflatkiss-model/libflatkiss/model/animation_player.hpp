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

#ifndef LIBFLATKISS_MODEL_ANIMATION_PLAYER_HPP_INCLUDED
#define LIBFLATKISS_MODEL_ANIMATION_PLAYER_HPP_INCLUDED

#include <cstdint>
#include <fstream>
#include <libflatkiss/model/animation.hpp>
#include <libflatkiss/model/sprite.hpp>
#include <unordered_map>

/**
 * @brief Maps sprite indices to their animated counterparts.
 *
 * Given a sprite and a tick, this class helps determining the sprite which
 * should be displayed.
 */
class AnimationPlayer {
 public:
  /**
   * @brief Construct an AnimationPlayer from a map of animations.
   *
   * @param animations_per_sprite Map of sprite indices to their respective
   * animations.
   */
  AnimationPlayer(
      std::unordered_map<Sprite, Animation>&& animations_per_sprite);
  Sprite const& animatedSpriteFor(Sprite const& sprite, int64_t tick) const;
  int64_t animationDurationForSprite(Sprite const& sprite) const;

 private:
  std::unordered_map<Sprite, Animation> const animations_per_sprite_;
};

#endif
