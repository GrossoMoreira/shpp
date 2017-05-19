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

#ifndef _SHPP_SERVICE_HPP_
#define _SHPP_SERVICE_HPP_

#include "cmd.h"
#include "exceptions.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>

namespace shpp
{
	class service {

		std::unordered_map<std::string, i_cmd*> commands;

		public:
			typedef std::unordered_map<std::string, i_cmd*>::const_iterator const_iterator;

			~service();

			template <typename R, typename ... T> void provide_command(std::string name, R(*func)(T...));
			template <typename C, typename R, typename ... T> void provide_command(std::string name, R(C::*func)(T...), C* object);
			template <typename L> void	provide_command(std::string name, L lambda);
			template <typename T> void provide_value(std::string name, T& var);

			void remove_command(std::string name);
			std::string call(std::string name, std::queue<std::string> args);
			const_iterator begin() const;
			const_iterator end() const;
	};

} // namespace

#endif // _SHPP_SERVICE_HPP_
