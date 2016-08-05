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

#ifndef _SHPP_PARAMETER_HPP_
#define _SHPP_PARAMETER_HPP_

#include <string>
#include "cast.h"

namespace shpp
{
	class parameter {

		std::string type;
		std::string name;

		parameter(std::string type = "", std::string name = "");

		public:
			template <typename T> static parameter make_parameter(std::string name = "");

			std::string get_type() const;
			std::string get_name() const;
	};
}

#endif // _SHPP_PARAMETER_HPP_
