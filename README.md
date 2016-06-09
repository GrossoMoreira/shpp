# shpp
Call c++ functions from a shell with any arguments of any types parsed automatically

Declare a variable or define a function and register it in shpp with a simple command. Your function will be available to be called by a shell that parses the arguments automatically to the types of the arguments of your function.

# features
- read and write variables at runtime (const/read-only variables supported)
- call functions with any number of arguments of any primitive types
- when a function is called, output its return value (of any primitive type)

# interface
```c++
#include "shpp/shpp.h"

shpp::service svc;	// create a service

svc.export("function_name", function_ptr);
svc.export("variable_name", variable_ref);

shpp::shell sh(svc);	// create a shell for this service
sh.start();		        //start interactive shell
```

# interactive shell
![Interactive shell demo](https://cloud.githubusercontent.com/assets/17955551/15941826/c7da29f4-2e79-11e6-9d9e-2826638cefdf.gif "Interactive shell demo")

# caveats
- function overloading not allowed, because we wouldn't know which overload to call
- only primitive types for variables, function arguments and function return values are supported, because we wouldn't know how to parse user defined types from/into strings. std::string is supported though, and more useful but simple types should be added soon

# todo
- use GNU readline library instead of current basic shell
- give names to types, so that we can at least list the argument types of each function
- add support for common types that may be useful, like std::vector<something>
- support arguments of user-defined types
- call functions remotely
