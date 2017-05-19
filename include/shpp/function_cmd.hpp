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

#ifndef _SHPP_FUNCTION_CMD_HPP_
#define _SHPP_FUNCTION_CMD_HPP_

#include "cast.h"
#include "cmd.h"
#include "exceptions.h"

#include <string>
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <functional>

namespace shpp
{
	template <typename Ret, typename ... FA> class function_cmd : public i_cmd {

		template <typename ... T> class converter {

			int argN;

			public:
				converter(int n);
				void for_each(std::function<void(const parameter&)>);
				template <typename ... A> std::string call(std::queue<std::string> q, std::function<Ret(FA...)> func, A... args) const;
		};

		template <typename Current, typename ... Next> class converter<Current, Next...> {

			int argN;
			converter<Next...> next;

			public:
				converter<Current, Next...>(int argN);
				void for_each(std::function<void(const parameter&)>);
				template <typename ... A> std::string call (std::queue<std::string> q, std::function<Ret(FA...)> func, A... args) const;
		};

		std::function<Ret(FA...)> func;
		converter<FA...> conv;

		public:
			function_cmd(std::string name, std::function<Ret(FA...)> func);
			i_cmd::form get_form() const;
			std::string get_return_type() const;
			std::string call(std::queue<std::string> q) const;
	};

} // namespace

#endif // _SHPP_FUNCTION_CMD_HPP_
