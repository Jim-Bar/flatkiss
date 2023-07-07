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

#include <libflatkiss/model/animation_player.hpp>
#include <utility>

using std::move;
using std::unordered_map;

AnimationPlayer::AnimationPlayer(
    unordered_map<uint16_t, Animation>&& animations_per_sprite_index)
    : animations_per_sprite_index_{move(animations_per_sprite_index)} {}

uint16_t AnimationPlayer::animatedSpriteIndexFor(uint16_t sprite_index,
                                                 int64_t tick) const {
  if (!animations_per_sprite_index_.contains(sprite_index)) {
    return sprite_index;
  }

  Animation const& animation{animations_per_sprite_index_.at(sprite_index)};
  return animation.spriteIndexAtStep(
      (tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}

int64_t AnimationPlayer::animationDurationForSpriteIndex(
    uint16_t sprite_index) const {
  if (!animations_per_sprite_index_.contains(sprite_index)) {
    return 0;
  }

  return animations_per_sprite_index_.at(sprite_index).getDuration();
}
