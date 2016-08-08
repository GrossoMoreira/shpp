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

double vec_sum(std::vector<double> v)
{
	double s = 0;
	for(double n : v)
		s += n;
	return s;
}

std::vector<int> count(int from, int to) {
	std::vector<int> v;
	for(; from <= to; ++from)
		v.push_back(from);

	return v;
}

// Example variable

int some_variable = 1337;

// Example use case

int main() {

	shpp::service svc;

	svc.provide("exception_test", exception_test);
	svc.provide("types_test", types_test);
	svc.provide("vec_sum", vec_sum);
	svc.provide("count", count);
	svc.provide("some_var", some_variable);

	shpp::shell sh(svc);
	sh.start();

	return 0;
}
