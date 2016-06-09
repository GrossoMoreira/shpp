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

#include "include/src/i_cmd.h"

shpp::i_cmd::i_cmd(std::string name, unsigned int num_args) : name(name), num_args(num_args) {
}

shpp::i_cmd::~i_cmd() {
}

std::string shpp::i_cmd::get_name() const {
	return name;
}

unsigned int shpp::i_cmd::expected_args() const {
	return num_args;
}
