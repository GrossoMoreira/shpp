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

#ifndef _SHPP_I_CMD_H_
#define _SHPP_I_CMD_H_

#include "parameter.hpp"

#include <string>
#include <vector>
#include <queue>

namespace shpp
{
	class i_cmd {

		public:
			enum form { variable, function };

			typedef std::vector<parameter> args_t;
			typedef args_t::iterator iterator;
			typedef args_t::const_iterator const_iterator;

		private:

			std::string name;
			unsigned int num_args;

			std::vector<parameter> params;

		protected:

			void add_parameter(const parameter&);

		public:

			i_cmd(std::string name);
			virtual ~i_cmd();

			virtual form get_form() const = 0;
			virtual std::string get_return_type() const = 0;

			std::string get_name() const;
			args_t::size_type size() const;

			iterator begin();
			iterator end();

			const_iterator cbegin() const;
			const_iterator cend() const;

			virtual std::string call(std::queue<std::string>) const = 0;
	};
}

#endif // _SHPP_I_CMD_H_
