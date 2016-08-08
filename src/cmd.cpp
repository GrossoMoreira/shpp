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

#include "shpp/cmd.h"

void shpp::i_cmd::add_parameter(const shpp::parameter& p) {
	params.push_back(p);
}

shpp::i_cmd::i_cmd(std::string name) : name(name) {
}

shpp::i_cmd::~i_cmd() {
}

std::string shpp::i_cmd::get_name() const {
	return name;
}

shpp::i_cmd::args_t::size_type shpp::i_cmd::size() const {
	return params.size();
}

shpp::i_cmd::iterator shpp::i_cmd::begin() {
	return params.begin();
}

shpp::i_cmd::iterator shpp::i_cmd::end() {
	return params.end();
}

shpp::i_cmd::const_iterator shpp::i_cmd::cbegin() const {
	return params.cbegin();
}

shpp::i_cmd::const_iterator shpp::i_cmd::cend() const {
	return params.cend();
}
