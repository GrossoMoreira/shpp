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
				
		char* result = (char*) malloc(sizeof(char) * cmd->get_name().size() + 1);

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

	switch(cmd->get_form()) {
		case i_cmd::variable:
			std::cout << bold << " - " << none << style_return << cmd->get_return_type() << none << " " << style_name << cmd->get_name() <<none << std::endl;
			break;

		case i_cmd::function:
			std::cout << bold << " - " <<  none << style_return << cmd->get_return_type() << " " << none << style_name << cmd->get_name() << none << "(" << none;
			
			bool first = true;
			for(const parameter& p : *cmd) {
				if(!first)
					std::cout << ", " << none;
				first = false;
				std::cout << style_param << p.get_type() << none;
			}

			std::cout << ")" << none << std::endl;
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

void shpp::shell::init_colors(color_mode mode)
{
	switch(mode)
	{
		case colors_enabled:
			style_return = cyan; 
			style_name = none;
			style_param = lightcyan;
			style_error = red;
			style_cout= green;
			break;

		case colors_disabled:
		default:
			style_return = "";
			style_name = "";
			style_param = "";
			style_error = "";
			style_cout = "";
	}

}

shpp::shell::shell(shpp::service& s, color_mode colors) :
	interactive(isatty(fileno(stdin))),
	svc(s)
{
	singleton = this;
	init_colors(colors);
}

std::string shpp::shell::execute(std::string cmd_name, std::queue<std::string> arguments, exec_result& code) {
	std::string result;

	try {
		std::cout << style_cout;
		result = svc.call(cmd_name, arguments);
		code = exec_ok;
		std::cout << none;
	} catch(shpp::cmd_not_found e) {
		std::cout << none;
		try {
			result = shell_commands(cmd_name, arguments, code);
		} catch (shpp::cmd_not_found e) {
			std::cout << style_error << "ERROR: command " << cmd_name << " not found." << none << std::endl;
			code = exec_error;
		}

	} catch(shpp::invalid_argument& e) {
		std::cout << style_error << "ERROR: argument " << e.argN << " is invalid." << none << std::endl;
		code = exec_error;
	} catch(shpp::out_of_range& e) {
		std::cout << style_error << "ERROR: argument " << e.argN << " is out of range." << none << std::endl;
		code = exec_error;
	} catch(shpp::no_cast_available& e) {
		std::cout << style_error << "ERROR: no cast avaliable for argument " << e.argN << "." << none << std::endl;
		code = exec_error;
	} catch(shpp::read_only_variable& e) {
		std::cout << style_error << "ERROR: variable is read-only." << none << std::endl;
		code = exec_error;
	} catch(shpp::wrong_argument_count& e) {
		std::cout << style_error << "ERROR: wrong argument count (expected " << e.expected << ", found " << e.provided << ")." << none << std::endl;
		code = exec_error;
	} catch (shpp::parse_exception& e) {
		std::cout << red << "ERROR: could not parse argument " << e.argN << " (" << e.value << ") . " << e.what() << none << std::endl;
		code = exec_error;
	} catch(shpp::command_exception& e) {
		std::cout << style_error << "ERROR: exception thrown by " << cmd_name;

		if(*e.what() == '\0')
			std::cout << '.';
		else
			std::cout << ": " << e.what();

		std::cout << none << std::endl;
		code = exec_error;
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
