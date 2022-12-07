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

#include <libflatkiss/model/animation.hpp>

using std::vector;

Animation::Animation(vector<uint16_t> const& sprite_indices, uint8_t period,
                     uint8_t duration)
    : sprite_indices_{move(sprite_indices)},
      period_{period},
      duration_{duration} {}

int64_t Animation::getDuration() const { return duration_; }

int64_t Animation::getPeriod() const { return period_; }

uint16_t Animation::spriteIndexAtStep(int64_t step) const {
  return sprite_indices_[step];
}
