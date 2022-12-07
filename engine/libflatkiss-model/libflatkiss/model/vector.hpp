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

#ifndef LIBFLATKISS_MODEL_VECTOR_HPP_INCLUDED
#define LIBFLATKISS_MODEL_VECTOR_HPP_INCLUDED

#include <cstdint>

/**
 * @brief A delta in pixels.
 *
 * Note that this is always in pixels. This class must not be used for things
 * measured in tiles.
 */
class Vector {
 public:
  Vector(int64_t dx, int64_t dy);
  bool operator!=(Vector const& other) const;
  bool operator==(Vector const& other) const;
  int64_t dx() const;
  int64_t dy() const;

  static Vector const kZero;

 private:
  int64_t const dx_;
  int64_t const dy_;
};

#endif
