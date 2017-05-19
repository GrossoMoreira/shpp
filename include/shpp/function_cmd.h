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

#ifndef _SHPP_FUNCTION_CMD_H_
#define _SHPP_FUNCTION_CMD_H_

#include "function_cmd.hpp"

#include "cast.h"
#include "parameter.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <functional>

template <typename Ret, typename ... FA>
template <typename ... T>
shpp::function_cmd<Ret, FA...>::converter<T...>::converter(int n) {}

template <typename Ret, typename ... FA>
template <typename ... T>
void shpp::function_cmd<Ret, FA...>::converter<T...>::for_each(std::function<void(const parameter&)> f) {
}

template <typename Ret, typename ... FA>
template <typename ... T>
template <typename ... A>
std::string shpp::function_cmd<Ret, FA...>::converter<T...>::call(std::queue<std::string> s, std::function<Ret(FA...)> func, A ... args) const {
	try {
		return call_to_string<Ret, FA...>(func, args...);
	} catch(std::exception& e) {
		throw command_exception(e.what());
	} catch(...) {
		throw command_exception();
	}
}

template <typename Ret, typename ... FA>
template <typename Current, typename ... Next>
shpp::function_cmd<Ret, FA...>::converter<Current,Next...>::converter(int n) : argN(n), next(n+1) {}

template <typename Ret, typename ... FA>
template <typename Current, typename ... Next>
void shpp::function_cmd<Ret, FA...>::converter<Current,Next...>::for_each(std::function<void(const parameter&)> f) {
	parameter p = parameter::make_parameter<Current>();
	f(p);
	next.for_each(f);
}

template <typename Ret, typename ... FA>
template <typename Current, typename ... Next>
template <typename ... A>
std::string shpp::function_cmd<Ret, FA...>::converter<Current,Next...>::call(std::queue<std::string> stack, std::function<Ret(FA...)> func, A ... args) const {
	std::string arg = stack.front();
	stack.pop();

	try {
		Current val = translator<Current>::parse(arg);
		return next.call(stack, func, args..., val);
	} catch (std::out_of_range) {
		throw out_of_range(argN, arg);
	} catch (std::invalid_argument& e) {
		throw argument_exception(argN, arg, "invalid argument");
	} catch (parse_exception& e) {
		e.argN = argN;
		e.value = arg;
		throw e;
	} catch (no_cast_available e) {
		if(e.argN > 0)
			throw e;
		throw no_cast_available(argN, arg);	
	} catch (command_exception e) {
		throw e;
	}
}

template <typename Ret, typename ... FA>
shpp::function_cmd<Ret, FA...>::function_cmd(std::string name, std::function<Ret(FA...)> func) : i_cmd(name), func(func), conv(1) {
	conv.for_each(
			[this](const parameter& p) {
				this->add_parameter(p);
			}
		);	
}

template <typename Ret, typename ... FA>
shpp::i_cmd::form shpp::function_cmd<Ret, FA...>::get_form() const {
	return i_cmd::function;
}

template <typename Ret, typename ... FA>
std::string shpp::function_cmd<Ret, FA...>::get_return_type() const {
	return translator<Ret>::name();
}

template <typename Ret, typename ... FA>
std::string shpp::function_cmd<Ret, FA...>::call(std::queue<std::string> q) const {
	if(q.size() != sizeof...(FA))
		throw wrong_argument_count(get_name(), sizeof...(FA), q.size());
        return conv.call(q, func);
}

#endif // _SHPP_FUNCTION_CMD_H_
