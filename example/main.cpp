#include "shpp/shpp.h"

#include <iostream>
#include <vector>

// Example functions

struct surprise {};

void exception_test() {
	std::cout << "This function will throw an exception!" << std::endl;
	throw surprise();
}

void types_test(int i, float f, std::string d) {
	std::cout << "int: " << i << ", float: " << f << ", string: " << d << std::endl;
}


std::vector<int> count(int from, int to) {
}

// Example variable

int some_variable = 1337;

// Example use case

struct sample_struct
{
	double vec_sum(std::vector<double> v)
	{
		double s = 0;
		for(double n : v)
			s += n;
		return s;
	}
};

int main() {
	shpp::service svc;

	svc.provide_command("exception_test", exception_test);
	svc.provide_command("types_test", types_test);

	sample_struct sum;
	svc.provide_command("vec_sum", &sample_struct::vec_sum, &sum);

	svc.provide_command("count", [](int from, int to) -> std::vector<int> {
								std::vector<int> v;
								for(; from <= to; ++from)
									v.push_back(from);

								return v;
							 });

	svc.provide_value("some_var", some_variable);

	shpp::shell sh(svc);
	sh.start();

	return 0;
}
