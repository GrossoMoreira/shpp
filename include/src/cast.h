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

#ifndef _SHPP_CAST_H_
#define _SHPP_CAST_H_

#include "exceptions.h"

#include <limits>
#include <stdexcept>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <iterator>
#include <vector>

namespace shpp
{
	// Parsing from string to argument type

	template <typename Received, typename Test> void test_num_limit(Received v) {
		if(v > std::numeric_limits<Test>::max())
			throw std::out_of_range("is above max numeric limit!");
		if(v < std::numeric_limits<Test>::min())
			throw std::out_of_range("is below numeric limit!");
	};

	template <typename T> T cast(std::string) throw(no_cast_available, std::out_of_range, std::invalid_argument)
	{
		throw no_cast_available(0, "");
	}

	template <> char cast<char>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> short cast<short>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> int cast<int>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> long cast<long>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> float cast<float>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> double cast<double>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> long double cast<long double>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> std::string cast<std::string>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);
	template <> const char* cast<const char*>(std::string s) throw(no_cast_available, std::out_of_range, std::invalid_argument);

	// Converting from argument type to string

	template <typename T> std::string to_string(const T& val) {
		std::stringstream ss;
		ss << val;
		return ss.str();
	}

	template <typename Ret> class devoid {
		public:
			template <typename ... Args> static std::string call(Ret(*f)(Args...), Args... args) {
				return to_string<Ret>(f(args...));
			}
	};

	template <> class devoid<void> {
		public:
			template <typename ... Args> static std::string call(void(*f)(Args...), Args... args) {
				f(args...);
				return std::string();
			}
	};

	template <typename Ret, typename ... Args> std::string call_to_string(Ret(*f)(Args...), Args... args) {
		return devoid<Ret>::call(f, args...);
	}
}

#endif // _SHPP_CAST_H_
