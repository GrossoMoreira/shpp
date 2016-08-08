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

#include "shpp/shell.h"
#include "shpp/exceptions.h"
#include "shpp/nargv.h"

#include <readline/readline.h>
#include <readline/history.h>

#include <stdio.h>
#include <unistd.h>

#include <memory>
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <iterator>
#include <string>
#include <queue>
#include <vector>

// -- Utility get line

const char* getline() {
	//std::cout << "getline START\n";
	std::vector<char> v;

	for(;;) {
		int c = fgetc(stdin);
		//std::cout << ((char) c) << std::endl;
		if(c == EOF)
			return nullptr;
		if(c == '\n')
			break;
		v.push_back((char) c);
	}
	
	if(!v.empty() && *(v.end() -1) != '\0')
		v.push_back('\0');

	char* a = new char(v.size());
	memcpy(a, v.data(), v.size());
	//std::cout << "getline END\n";
	return a;
}

// -- //

shpp::shell* shpp::shell::singleton = nullptr;

char** shpp::shell::completion_function(const char* text, int start, int end) {
	if(start != 0)
		return nullptr;

	return rl_completion_matches(text, shpp::shell::completion_matches_cb);
}

char* shpp::shell::completion_matches_cb(const char* text, int state) {
	if(!singleton)
		return nullptr;

	static service::const_iterator it;

	if(state == 0)
		it = singleton->get_service().begin();

	service::const_iterator et = singleton->get_service().end();

	std::string search(text);
	for(; it != et;) {
		const shpp::i_cmd* cmd = (*it).second;
		++it;
		std::string cmd_name = cmd->get_name();

		bool match = true;
		for(unsigned int i = 0; i < search.size(); ++i) {
			if(search[i] != cmd_name[i]) {
				match = false;
			}
		}

		if(!match)
			continue;
				
		char* result = (char*) malloc(sizeof(char) * cmd->get_name().size());

		if(!result)
			continue;

		strcpy(result, cmd->get_name().c_str());
		return result;
	}

	return nullptr;
}

const std::string shpp::shell::exit = "exit";
const std::string shpp::shell::help = "help";
const std::string shpp::shell::about = "about";
const std::string shpp::shell::source= "source";

void shpp::shell::print_about() {
	std::cout << "shpp library version 1.0, Copyright (C) 2016 Pedro Moreira" << std::endl;
	std::cout << "The shpp library is distributed in the hope that it will be useful," << std::endl;
	std::cout << "but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
	std::cout << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
	std::cout << "GNU General Public License for more details.\n" << std::endl;
}

void shpp::shell::print_signature(i_cmd* cmd) {

	std::string name_style;
	std::string return_style;
	std::string param_style;
	std::string punct_style;

	switch(colors) {
		case colors_disabled:
			name_style = none;
			return_style = none;
			param_style = none;
			punct_style = none;
			break;

		case colors_enabled:
			name_style = none;
			return_style = cyan;
			param_style = lightcyan;
			punct_style = none;
			break;

		default:
			name_style = none;
			return_style = none;
			param_style = none;
			punct_style = none;
			break;
	}

	switch(cmd->get_form()) {
		case i_cmd::variable:
			std::cout << bold << " - " << none << return_style << cmd->get_return_type() << none << " " << name_style << cmd->get_name() <<none << std::endl;
			break;

		case i_cmd::function:
			std::cout << bold << " - " <<  none << return_style << cmd->get_return_type() << " " << none << name_style << cmd->get_name() << none << punct_style << "(" << none;
			
			bool first = true;
			for(const parameter& p : *cmd) {
				if(!first)
					std::cout << punct_style << ", " << none;
				first = false;
				std::cout << param_style << p.get_type() << none;
			}

			std::cout << punct_style << ")" << none << std::endl;
			break;

	}
}

void shpp::shell::print_help() {
		std::cout << "List of available commands:\n";
		std::cout << " - help" << std::endl;
		std::cout << " - about" << std::endl;
		std::cout << " - exit" << std::endl;
		std::cout << " - source [FILE...]" << std::endl;
		std::cout << std::endl;

		for (auto& p : svc) {
			print_signature(p.second);
		}

		std::cout << std::endl;
}

void shpp::shell::source_files(std::queue<std::string> file_names) {

	bool interactive_status = interactive;
	interactive = false;

	while(!file_names.empty()) {
		std::string file_name = file_names.front();
		file_names.pop();

		std::ifstream infile(file_name);
		std::string line;
		std::getline(infile, line, '\n');
		while(!line.empty()) {
			if(eval(line.c_str()))
				break;
			std::getline(infile, line, '\n');
		}
	}

	interactive = interactive_status;
}

shpp::shell::shell(shpp::service& s, color_mode colors) :
	interactive(isatty(fileno(stdin))),
	svc(s),
	colors(colors) {
	singleton = this;
}

std::string shpp::shell::execute(std::string cmd_name, std::queue<std::string> arguments, exec_result& code) {
	std::string result;

	try {
		std::cout << shpp::shell::green;
		result = svc.call(cmd_name, arguments);
		std::cout << shpp::shell::none;
	} catch(shpp::cmd_not_found e) {
		std::cout << shpp::shell::none;
		try {
			result = shell_commands(cmd_name, arguments, code);
		} catch (shpp::cmd_not_found e) {
			std::cout << shpp::shell::red << "ERROR: command " << cmd_name << " not found." << shpp::shell::none << std::endl;
		}

	} catch(shpp::invalid_argument e) {
		std::cout << shpp::shell::red << "ERROR: argument " << e.argN << " is invalid." << shpp::shell::none << std::endl;
	} catch(shpp::out_of_range e) {
		std::cout << shpp::shell::red << "ERROR: argument " << e.argN << " is out of range." << shpp::shell::none << std::endl;
	} catch(shpp::no_cast_available e) {
		std::cout << shpp::shell::red << "ERROR: no cast avaliable for argument " << e.argN << "." << shpp::shell::none << std::endl;
	} catch(shpp::read_only_variable e) {
		std::cout << shpp::shell::red << "ERROR: variable is read-only." << shpp::shell::none << std::endl;
	} catch(shpp::wrong_argument_count e) {
		std::cout << shpp::shell::red << "ERROR: wrong argument count (expected " << e.expected << ", found " << e.provided << ")." << shpp::shell::none << std::endl;
	} catch (shpp::parse_exception& e) {
		std::cout << red << "ERROR: could not parse argument " << e.argN << " (" << e.value << ") . " << e.what() << none << std::endl;
	} catch(shpp::command_exception& e) {
		std::cout << shpp::shell::red << "ERROR: exception thrown by " << cmd_name << "." << shpp::shell::none << std::endl;
	}

	return result;
}

std::string shpp::shell::shell_commands(std::string cmd, std::queue<std::string> arguments, exec_result& code) {
	std::string ret;

	if(cmd == exit) {
		code = exec_exit;
	} else if(cmd == help) {
		print_help();
		code = exec_ok;
	} else if(cmd == about) {
		print_about();
		code = exec_ok;
	} else if(cmd == source) {
		source_files(arguments);
	} else {
		throw cmd_not_found(cmd);
	}

	return ret;
}

const shpp::service& shpp::shell::get_service() {
	return svc;
}

bool shpp::shell::eval(const char* line) {
	if(!line) {
		if(interactive)
			std::cout << std::endl;
		return true;
	}

	std::unique_ptr<NARGV, void(*)(NARGV*)> parsed_args(nargv_parse(line), nargv_free);

	std::string full_command(line);

	if(interactive && !full_command.empty() && full_command != last_command)
		add_history(full_command.c_str());

	if(!full_command.empty())
		last_command = full_command;

	switch(parsed_args->error_code) {
		case 1: // (provided char* was null)
			// this never happens because we checked line for null
			break;

		case 2: // (string was empty)
			// in this case we simply don't do anything
			return false;

		case 3: // (unterminated double quote)
			std::cout << "Syntax error: " << parsed_args->error_message << " at column " << parsed_args->error_index << std::endl;
			return false;

		case 4: // (unterminated single quote)
			std::cout << "Syntax error: " << parsed_args->error_message << " at column " << parsed_args->error_index << std::endl;
			return false;
	}

	std::string cmd_name(parsed_args->argv[0]);
	if(!cmd_name.empty() && cmd_name[0] == '#')
		return false;

	std::queue<std::string> args;

	for (int i = 1; i < parsed_args->argc; ++i)
		args.push(parsed_args->argv[i]);

	exec_result code;
	std::string result = execute(cmd_name, args, code);

	if(!result.empty())
		std::cout << result << std::endl;

	if(code == exec_exit)
		return true;
	
	return false;
}

void shpp::shell::start() {

	rl_attempted_completion_function = completion_function;

	while(true) { // main loop
		const char* line = interactive ? readline(">") : getline();
		bool exit = eval(line);
		free((void*)line);
		if(exit)
			break;
	}
}
