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

#ifndef _SHPP_I_CMD_H_
#define _SHPP_I_CMD_H_

#include "cast.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>

namespace shpp
{
	class i_cmd {

		std::string name;
		unsigned int num_args;

		public:
			i_cmd(std::string, unsigned int);
			virtual ~i_cmd();

			std::string get_name() const;
			unsigned int expected_args() const;

			virtual std::string call(std::queue<std::string>) const = 0;
	};

}

#endif // _SHPP_I_CMD_H_
