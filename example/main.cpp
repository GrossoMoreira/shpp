#include "include/shpp.h"

#include <iostream>

struct surprise {};

void exception_test() {
	std::cout << "This function will throw an exception!" << std::endl;
	throw surprise();
}

void types_test(int i, float f, std::string d) {
	std::cout << "int: " << i << ", float: " << f << ", string: " << d << std::endl;
}

double sum(double a, double b) {
	return a + b;
}

int some_variable;

int main() {
	shpp::service svc;

	svc.provide("exception_test", exception_test);
	svc.provide("types_test", types_test);
	svc.provide("sum", sum);
	svc.provide("some_var", some_variable);

	shpp::shell sh(svc);
	sh.start();

	return 0;
};
