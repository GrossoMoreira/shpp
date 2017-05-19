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
#include <functional>
#include <type_traits>

namespace {
	template <typename Function>
	struct function_traits : public function_traits<decltype(&Function::operator())> {};

	template <typename ClassType, typename ReturnType, typename... Args>
	struct function_traits<ReturnType(ClassType::*)(Args...) const> {
		 using return_type = ReturnType;
		 using pointer = ReturnType (*)(Args...);
		 using std_function = std::function<ReturnType(Args...)>;
	};

	template <typename Function>
	typename function_traits<Function>::std_function to_function (Function& lambda) {
		 return typename function_traits<Function>::std_function(lambda);
	}

	template <template <typename ...> class F, typename R, typename ... T>
	shpp::function_cmd<R, T...>* mk_function_cmd(std::string name, F<R(T...)> func)
	{
		return new shpp::function_cmd<R, T...>(name, func);
	}
}

template <typename R, typename ... T>
void shpp::service::provide_command(std::string name, R(*funcPtr)(T...)) {
	std::function<R(T...)> func(funcPtr);
	commands[name] = new function_cmd<R, T...>(name, func);
}

template <typename C, typename R, typename ... T>
void shpp::service::provide_command(std::string name, R(C::*funcPtr)(T...), C* object) {
	std::function<R(T...)> func([=](T... args) -> R {
			return ((*object).*funcPtr)(args...);
		});
	commands[name] = new function_cmd<R, T...>(name, func);
}

template <typename L>
void shpp::service::provide_command(std::string name, L lambda) {
	auto func = to_function(lambda);
	commands[name] = mk_function_cmd(name, func);
}

template <typename T>
void shpp::service::provide_value(std::string name, T& var) {
	commands[name] = new variable_cmd<T>(name, var);
}

#endif // _SHPP_SERVICE_H_
