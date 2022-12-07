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

#ifndef LIBFLATKISS_MODEL_ANIMATION_HPP_INCLUDED
#define LIBFLATKISS_MODEL_ANIMATION_HPP_INCLUDED

#include <cstdint>
#include <vector>

/**
 * @brief A list of sprite indices making up an animated sprite.
 *
 * An animation is a list of sprites displayed one after another. The period of
 * the animation is the number of sprites making it up. The duration is the
 * number of ticks a particular sprite is displayed before showing the next one.
 */
class Animation {
 public:
  Animation(std::vector<uint16_t> const& sprite_indices, uint8_t period,
            uint8_t Duration);
  int64_t getDuration() const;
  int64_t getPeriod() const;
  uint16_t spriteIndexAtStep(int64_t step) const;

 private:
  uint8_t const duration_;
  uint8_t const period_;
  std::vector<uint16_t> const sprite_indices_;
};

#endif
