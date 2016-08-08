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

#include "shpp/exceptions.h"

shpp::argument_exception::argument_exception() : std::exception() {
}

shpp::argument_exception::argument_exception(std::string s) :
	std::exception(),
	argN(0),
	explanation(s)
{
}

shpp::argument_exception::argument_exception(unsigned int argN, std::string value, std::string what) :
	std::exception(),
	argN(argN),
	value(value),
	explanation(what)
{
}

const char* shpp::argument_exception::what() const noexcept {
	return explanation.c_str();
}

shpp::no_cast_available::no_cast_available() : argument_exception() {
}

shpp::no_cast_available::no_cast_available(unsigned int argN, std::string value) : argument_exception(argN, value) {
}

shpp::out_of_range::out_of_range(unsigned int argN, std::string value) : argument_exception(argN, value) {
}

shpp::invalid_argument::invalid_argument(unsigned int argN, std::string value) : argument_exception(argN, value) {
}

shpp::cmd_not_found::cmd_not_found(std::string cmd) : command(cmd) {
}

shpp::wrong_argument_count::wrong_argument_count(std::string command, unsigned int expected, unsigned int provided) : command(command), expected(expected), provided(provided) {
}

shpp::parse_exception::parse_exception(std::string what) : argument_exception(what) {
}
