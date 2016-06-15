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

#ifndef _SHPP_VARIABLE_CMD_H_
#define _SHPP_VARIABLE_CMD_H_

#include "cast.h"
#include "exceptions.h"
#include "variable_cmd.hpp"

#include <string>
#include <queue>
#include <iostream>

template <typename T> shpp::variable_cmd<T>::variable_cmd(std::string name, T& var) : i_cmd(name, 1), variable(var) {
}

template <typename T> std::string shpp::variable_cmd<T>::call(std::queue<std::string> q) const throw(out_of_range, no_cast_available, wrong_argument_count, invalid_argument) {
	if(q.empty())
		return shpp::to_string(variable);
	else if (q.size() > 1)
		throw wrong_argument_count(get_name(), 1, q.size());
	else
		try {
			variable = cast<T>(q.front());
		} catch (std::out_of_range e) {
			throw out_of_range (1, q.front());
		} catch (std::invalid_argument) {
			throw invalid_argument(1, q.front());
		} catch (no_cast_available) {
			throw no_cast_available(1, q.front());
		}

	return "";
}

template <typename T> shpp::variable_cmd<const T>::variable_cmd(std::string name, const T& var) : i_cmd(name, 0), variable(var) {
}

template <typename T> std::string shpp::variable_cmd<const T>::call(std::queue<std::string> q) const throw(out_of_range, no_cast_available, wrong_argument_count, read_only_variable) {
	if(q.empty())
		return shpp::to_string<T>(variable);
	
	throw read_only_variable();
}

#endif // _SHPP_VARIABLE_CMD_H_
