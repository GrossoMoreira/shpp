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

#ifndef _SHPP_SERVICE_H_
#define _SHPP_SERVICE_H_

#include "service.hpp"

#include "function_cmd.h"
#include "variable_cmd.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>

template <typename R, typename ... T>
void shpp::service::provide(std::string name, R(*func)(T...)) {
	commands[name] = new function_cmd<R, T...>(name, func);
}

template <typename T>
void shpp::service::provide(std::string name, T& var) {
	commands[name] = new variable_cmd<T>(name, var);
}

#endif // _SHPP_SERVICE_H_
