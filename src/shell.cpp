/*
   Copyright (C) 2016  Pedro Moreira

   shpp library: call c++ functions of a running program from a shell

   This program is free software; you can shpp::shell::redistribute it and/or modify
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

#include "Console.hpp" // cpp-readline

#include <stdio.h>
#include <unistd.h>

#include <iostream>
#include <istream>
#include <sstream>
#include <iterator>
#include <string>
#include <queue>
#include <vector>

namespace cr = CppReadline;
using ret_code = cr::Console::ReturnCode;

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


static int exec_command(const shpp::i_cmd* cmd, std::queue<std::string> arguments) {
	std::string result;
	try {
		std::cout << shpp::shell::green;
		result = cmd->call(arguments);
		std::cout << shpp::shell::none;
	} catch(shpp::invalid_argument e) {
		std::cout << shpp::shell::red << "ERROR: argument " << e.argN << " is invalid" << shpp::shell::none << std::endl;
	} catch(shpp::out_of_range e) {
		std::cout << shpp::shell::red << "ERROR: argument " << e.argN << " is out of range" << shpp::shell::none << std::endl;
	} catch(shpp::no_cast_available e) {
		std::cout << shpp::shell::red << "ERROR: no cast avaliable for argument " << e.argN << shpp::shell::none << std::endl;
	} catch(shpp::read_only_variable e) {
		std::cout << shpp::shell::red << "ERROR: variable is read-only" << shpp::shell::none << std::endl;
	} catch(shpp::wrong_argument_count e) {
		std::cout << shpp::shell::red << "ERROR: wrong argument count (expected " << e.expected << ", found " << e.provided << ")" << shpp::shell::none << std::endl;
	} catch(shpp::command_exception e) {
		std::cout << shpp::shell::red << "ERROR: exception thrown by " << cmd->get_name() << shpp::shell::none << std::endl;
	}

	if(result.size() != 0)
		std::cout << result << std::endl;

	return 0;
}

void shpp::shell::start() {
	bool interactive = isatty(fileno(stdin));

	if(interactive)
		print_about();

	cr::Console c(interactive ? ">" : "");

	c.registerCommand(about,
		[this](const cr::Console::Arguments& args) -> int {
			print_about();
			return 0;
		});

	c.registerCommand(help,
		[this](const cr::Console::Arguments& args) -> int {
			std::cout << "Available commands:\n";
			for(auto p : svc) {
				const i_cmd* cmd = p.second;
				std::cout << " - " << cmd->get_name() << " (" << cmd->expected_args() << " arg" << (cmd->expected_args() != 1 ? "s" : "") << ")" << std::endl;
			}
			return 0;
		});


	for(auto p : svc) {
		std::string cmd_name = p.first;
		const i_cmd* cmd = p.second;

		c.registerCommand(cmd_name,
			[this, cmd](const cr::Console::Arguments& args)-> int { 
				std::queue<std::string> q;

				for(const std::string& s : args)
					q.push(s);

				q.pop(); // the first element is the name of the command, so remove it

				return exec_command(cmd, q);
			}
		);
	}

	while(true)
		if(c.readLine() == ret_code::Quit)
			break;
}
