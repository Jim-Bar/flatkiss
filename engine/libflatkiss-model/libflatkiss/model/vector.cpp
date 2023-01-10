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

#include <libflatkiss/model/vector.hpp>

Vector const Vector::kZero{0, 0};

Vector::Vector(int64_t dx, int64_t dy) : dx_{dx}, dy_{dy} {}

bool Vector::operator!=(Vector const& other) const { return !(*this == other); }

bool Vector::operator==(Vector const& other) const {
  return dx() == other.dx() && dy() == other.dy();
}

int64_t Vector::dx() const { return dx_; }

int64_t Vector::dy() const { return dy_; }
