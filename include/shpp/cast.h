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

#ifndef _SHPP_CAST_H_
#define _SHPP_CAST_H_

#include "type_name.h"
#include "exceptions.h"

#include <map>

#include "jsoncons/json.hpp"

#include <limits>
#include <functional>
#include <stdexcept>
#include <string>
#include <sstream>
#include <istream>
#include <iostream>
#include <iterator>
#include <tuple>
#include <vector>
#include <typeinfo>
#include <cxxabi.h>

namespace shpp
{
	// Parsing from string to argument type

	template <typename Received, typename Test> void test_num_limit(Received v) {
		if(v > std::numeric_limits<Test>::max())
			throw std::out_of_range("is above max numeric limit!");
		if(v < std::numeric_limits<Test>::min())
			throw std::out_of_range("is below numeric limit!");
	};

	template <typename T> T cast(std::string)
	{
		throw no_cast_available(0, "");
	}

	template <> char cast<char>(std::string s);
	template <> short cast<short>(std::string s);
	template <> int cast<int>(std::string s);
	template <> long cast<long>(std::string s);
	template <> float cast<float>(std::string s);
	template <> double cast<double>(std::string s);
	template <> long double cast<long double>(std::string s);
	template <> std::string cast<std::string>(std::string s);
	template <> const char* cast<const char*>(std::string s);

	// Converting from argument type to string

	template <typename T> std::string to_string(const T& val) {
		std::stringstream ss;
		ss << val;
		return ss.str();
	}

	// Converting from parameter type to string

	template <typename T> std::string type_str() {
		static_string s = type_name<T>();
		std::string n = std::string(s.data(), s.size());
		std::string m = "std::basic_string<char>";
		size_t p = n.find(m);
		if(p != std::string::npos) {
			std::string r = "std::string";
			n.replace(p, m.size(), r);
		}
		return n;
	}

	// Converters between template structures and string

	using json = jsoncons::json;

	template <typename T> struct translator {
		typedef T type;

		static void build_json(json& j, const T& v) {
			j.add(v);
		}

		static std::string to_str(const T& v) {
			return to_string(v);
		}

		static T parse(std::string s) {
			return cast<T>(s);
		}

		static std::string name() {
			return type_str<T>();
		}
	};

	template <template <typename...> class C, typename F, typename ... E> struct translator<C<F, E...>> {
		typedef F type;

		static void build_json(json& j, const C<F, E...>& c) {
			json k = json::array();
			for(auto& v : c)
				translator<F>::build_json(k, v);
			j.add(k);
		}

		static std::string to_str(const C<F, E...>& c) {
			json j = json::array();
			build_json(j, c);
			json f = j[0];
			std::stringstream ss;
			ss << pretty_print(f);
			return ss.str();
		}

		static C<F, E...> parse(std::string s) {

			try {
				C<F, E...> c;

				json j = json::parse(s);

				for(auto& e : j.elements()) {
					std::stringstream ss;
					ss << pretty_print(e);
					c.push_back(translator<F>::parse(ss.str()));
				}

				return c;

			} catch (jsoncons::parse_exception& e) {
				throw parse_exception(e.what());
			}
		}

		static std::string name() {
			return type_str<C<F,E...>>();
		}
	};

	template <> struct translator<std::string> {
		typedef std::string type;

		static void build_json(json& j, const std::string& s) {
			j.add(s);
		}

		static std::string to_str(const std::string& s) {
			return s;
		}

		static std::string parse(std::string s) {
			auto b = s.cbegin();
			auto e = s.cend();

			if(s.size() > 1 && *b == '\"' && *(e-1) == '\"')
				return std::string(b + 1, e - 1);

			return s;
		}

		static std::string name() {
			return type_str<std::string>();
		}
	};

	template <> struct translator<void> {
		typedef void type;

		static std::string name() {
			return type_str<void>();
		}
	};

	// Calling void and non-void functions

	template <typename Ret> class devoid {
		public:
			template <typename ... Args> static std::string call(std::function<Ret(Args...)> f, Args... args) {
				return translator<Ret>::to_str(f(args...));
			}
	};

	template <> class devoid<void> {
		public:
			template <typename ... Args> static std::string call(std::function<void(Args...)> f, Args... args) {
				f(args...);
				return std::string();
			}
	};

	template <typename Ret, typename ... Args> std::string call_to_string(std::function<Ret(Args...)> f, Args... args) {
		return devoid<Ret>::call(f, args...);
	}


}

#endif // _SHPP_CAST_H_
