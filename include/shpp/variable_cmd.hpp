/*
   Copyright (C) 2016  Pedro Moreira

   shpp library: call c++ functions of a running program from a shell

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#ifndef _SHPP_VARIABLE_CMD_HPP_
#define _SHPP_VARIABLE_CMD_HPP_

#include "cmd.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>

namespace shpp
{
	template <typename T> class variable_cmd : public i_cmd {

		T& variable;

		public:
			variable_cmd(std::string name, T& var);
			i_cmd::form get_form() const;
			std::string get_return_type() const;
			std::string call(std::queue<std::string>) const;
	};

	template <typename T> class variable_cmd<const T> : public i_cmd {

		const T& variable;

		public:
			variable_cmd(std::string name, const T& var);
			i_cmd::form get_form() const;
			std::string get_return_type() const;
			std::string call(std::queue<std::string>) const;

	};

}

#endif // _SHPP_VARIABLE_CMD_HPP_
