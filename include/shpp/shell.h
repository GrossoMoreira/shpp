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

#ifndef _SHPP_SHELL_H_
#define _SHPP_SHELL_H_

#include "service.hpp"
#include "exceptions.h"

namespace shpp
{
	class shell {
		public:
			enum color_mode { colors_enabled, colors_disabled };

		private:

			enum exec_result { exec_ok, exec_error, exec_exit };

			// singleton
			static shell* singleton;

			// special commands
			static const std::string exit;
			static const std::string help;
			static const std::string about;
			static const std::string source;

			static char** completion_function(const char* buff, int start, int end);
			static char* completion_matches_cb(const char* text, int state);

			std::string execute(std::string command, std::queue<std::string> args, exec_result&);
			std::string shell_commands(std::string command, std::queue<std::string> args, exec_result&);

			bool eval(const char* line);

			void print_about();
			void print_signature(i_cmd*);
			void print_help();
			void source_files(std::queue<std::string> file_names);

			bool interactive;
			service& svc;
			std::string last_command;

			// colors
			std::string style_return;
			std::string style_name;
			std::string style_param;
			std::string style_error;
			std::string style_cout;

			void init_colors(color_mode);

		public:

			// colors
			typedef const char* color;
			static constexpr color none = "\e[0m";
			static constexpr color red = "\e[91m";
			static constexpr color cyan = "\e[38;5;45m";	
			static constexpr color lightcyan = "\e[38;5;117m";	
			static constexpr color blue = "\e[38;5;6m";
			static constexpr color orange = "\e[38;5;208m";
			static constexpr color yellow = "\e[93m";
			static constexpr color green = "\e[38;5;118m";
			static constexpr color pink = "\e[95m";	

			static constexpr color bold= "\e[1m";

			shell (service&, color_mode colors = colors_enabled);
			const service& get_service();
			void start();
	};
}

#endif // _SHPP_SHELL_H_
