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
    unordered_map<Sprite, Animation>&& animations_per_sprite)
    : animations_per_sprite_{move(animations_per_sprite)} {}

Sprite const& AnimationPlayer::animatedSpriteFor(Sprite const& sprite,
                                                 int64_t tick) const {
  if (!animations_per_sprite_.contains(sprite)) {
    return sprite;
  }

  Animation const& animation{animations_per_sprite_.at(sprite)};
  return animation.spriteAtStep(
      (tick % (animation.getPeriod() * animation.getDuration())) /
      animation.getDuration());
}

int64_t AnimationPlayer::animationDurationForSprite(
    Sprite const& sprite) const {
  if (!animations_per_sprite_.contains(sprite)) {
    return 0;
  }

  return animations_per_sprite_.at(sprite).getDuration();
}
