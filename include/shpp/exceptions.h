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

#ifndef _SHPP_EXCEPTIONS_HPP_
#define _SHPP_EXCEPTIONS_HPP_

#include <string>
#include <exception>

namespace shpp
{
	struct argument_exception : public std::exception
	{
		unsigned int argN;
		std::string value;
		std::string explanation;

		argument_exception();
		argument_exception(std::string what);
		argument_exception(unsigned int, std::string, std::string what = "");

		const char* what() const noexcept;
	};

	struct no_cast_available : public argument_exception {
		no_cast_available();
		no_cast_available(unsigned int, std::string);
	};

	struct out_of_range : public argument_exception {
		out_of_range(unsigned int, std::string);
	};

	struct invalid_argument : public argument_exception {
		invalid_argument(unsigned int, std::string);
	};

	struct cmd_not_found {
		std::string command;
		cmd_not_found(std::string);
	};

	struct read_only_variable {
	};

	struct parse_exception : public argument_exception {
		parse_exception(std::string what);
	};

	struct wrong_argument_count {
		std::string command;
		unsigned int expected;
		unsigned int provided;
	
		wrong_argument_count(std::string, unsigned int, unsigned int);
	};
	
	struct command_exception {

		std::string whatstr;

		command_exception();
		command_exception(std::string);

		const char* what() const noexcept;
	};

} // namespace

#endif // _SHPP_EXCEPTIONS_HPP_
