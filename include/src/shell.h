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

#ifndef _SHPP_SHELL_H_
#define _SHPP_SHELL_H_

#include "service.hpp"

namespace shpp
{
	class shell {

		// special commands
		static const std::string exit;
		static const std::string help;
		static const std::string about;

		service& svc;

		public:

		// colors
		typedef const char* color;
		static constexpr color none = "\e[0m";		
		static constexpr color red = "\e[91m";		
		static constexpr color green = "\e[92m";		


		shell (service&);
		void start();
	};
}

#endif // _SHPP_SHELL_H_
