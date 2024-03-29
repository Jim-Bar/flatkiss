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

#ifndef LIBFLATKISS_LOGIC_LOGIC_HPP_INCLUDED
#define LIBFLATKISS_LOGIC_LOGIC_HPP_INCLUDED

#include <libflatkiss/logic/character_controller.hpp>
#include <libflatkiss/logic/character_controller_loader.hpp>
#include <libflatkiss/logic/keyboard_character_controller.hpp>
#include <libflatkiss/logic/navigator.hpp>
#include <libflatkiss/logic/stroll_character_controller.hpp>
#include <memory>
#include <vector>

class Logic {
 public:
  Logic(std::vector<std::unique_ptr<CharacterController>>&&
            character_controllers,
        Navigator const& navigator);
  Navigator const& navigator() const;  // FIXME: Delete.
  std::vector<std::unique_ptr<CharacterController>>& characterControllers();

 private:
  std::vector<std::unique_ptr<CharacterController>> character_controllers_;
  Navigator const navigator_;
};

#endif
