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

#include "shpp/cast.h"

template <>
char shpp::cast<char>(std::string s) {
        if(s.size() > 0)
                return s[0];
        else
                throw std::out_of_range("string to char: expected a character but the argument was empty");
}

template <>
short shpp::cast<short>(std::string s) {
        int v = std::stoi(s);
        test_num_limit<int,short>(v);
        return (short) v;
}

template <>
int shpp::cast<int>(std::string s) {
	return std::stoi(s);
}

template <>
long shpp::cast<long>(std::string s) {
	return std::stol(s);
}

template <>
float shpp::cast<float>(std::string s) {
	return std::stof(s);
}

template <>
double shpp::cast<double>(std::string s) {
	return std::stod(s);
}

template <>
long double shpp::cast<long double>(std::string s) {
	return std::stold(s);
}

template <>
std::string shpp::cast<std::string>(std::string s) {
	return s;
}

template <>
const char* shpp::cast<const char*>(std::string s) {
	return s.c_str();
}
