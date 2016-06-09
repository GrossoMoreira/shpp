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

#include "include/src/shell.h"
#include "include/src/exceptions.h"

#include <iostream>
#include <istream>
#include <sstream>
#include <iterator>
#include <string>
#include <queue>
#include <vector>

const std::string shpp::shell::exit = "exit";
const std::string shpp::shell::help = "help";
const std::string shpp::shell::about = "about";

static void print_about() {
	std::cout << "shpp library version 1.0, Copyright (C) 2016 Pedro Moreira" << std::endl;
	std::cout << "The shpp library is distributed in the hope that it will be useful," << std::endl;
	std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
	std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
	std::cout << "GNU General Public License for more details.\n" << std::endl;
}

shpp::shell::shell(shpp::service& s) : svc(s) {
}

std::string shpp::shell::call(std::string command, std::queue<std::string> arguments) {
	std::string result;
	try {
		std::cout << green;
		result = svc.call(command, arguments);
		std::cout << none;
	} catch(invalid_argument e) {
		std::cerr << red << "ERROR: argument " << e.argN << " is invalid" << none << std::endl;
	} catch(out_of_range e) {
		std::cerr << red << "ERROR: argument " << e.argN << " is out of range" << none << std::endl;
	} catch(no_cast_available e) {
		std::cerr << red << "ERROR: no cast avaliable for argument " << e.argN << none << std::endl;
	} catch(cmd_not_found e) {
		std::cerr << red << "ERROR: command \"" << e.command << "\" not found" << none << std::endl;
	} catch(read_only_variable e) {
		std::cerr << red << "ERROR: variable is read-only" << none << std::endl;
	} catch(wrong_argument_count e) {
		std::cerr << red << "ERROR: wrong argument count (expected " << e.expected << ", found " << e.provided << ")" << none << std::endl;
	} catch(command_exception e) {
		std::cerr << red << "ERROR: exception thrown by " << command << none << std::endl;
	}
	return result;
}

void shpp::shell::start() {

	std::cin.clear();

	while(true) {
		std::string in;

		std::cout << '>';
		std::getline(std::cin, in);

		if(in.substr(0, exit.size()) == exit)
			break;

		if(in.substr(0, help.size()) == help)
		{
			for(auto p : svc) {
				const i_cmd* cmd = p.second;
				std::cout << " - " << cmd->get_name() << " (" << cmd->expected_args() << " arg" << (cmd->expected_args() != 1 ? "s" : "") << ")" << std::endl;
			}
			continue;
		}

		if(in.substr(0, about.size()) == about) {
			print_about();
			continue;
		}

		if(in.size() > 0) {
			std::stringstream ss(in);
			std::istream_iterator<std::string> it(ss);
			std::istream_iterator<std::string> end;

			std::vector<std::string> tokens(it, end);
			if(tokens.size() == 0) {
				std::cerr << red << "ERROR: unable to parse command" << none << std::endl;
				continue;
			}
			std::string command = tokens[0];
			tokens.erase(tokens.begin());

			std::queue<std::string> arguments;
			for(auto s : tokens)
				arguments.push(s);

			std::string result = call(command, arguments);
			if(result.size() != 0)
				std::cout << result << std::endl;
		}

		if(std::cin.eof()) {
			std::cout << std::endl;
			break;
		}
	}
}
