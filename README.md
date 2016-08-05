# shpp
Call c++ functions from a shell with any arguments of any types parsed automatically

Declare a variable or define a function and register it in shpp with a simple command. Your function will be available to be called by a shell that parses the arguments automatically to the types of the arguments of your function.

# Features
- list available functions, including their return type and parameter types
- call functions with any number of arguments of any primitive types or stl-like non-associative containers
- read and write variables (const/read-only variables supported)
- when a function is called, output its return value (of any supported type)
- interactive and non interactive mode
- auto-complete
- command history
- reverse search
- sourcing commands from files

# Interface
```c++
#include "shpp/shpp.h"

shpp::service svc;	// create a service

svc.provide("function_name", function_ptr);
svc.provide("variable_name", variable_ref);

shpp::shell sh(svc);	// create a shell for this service
sh.start();			// start shell
```

# Interactive shell
![Interactive shell demo](https://cloud.githubusercontent.com/assets/17955551/17453191/c089d6c2-5b6f-11e6-9c1c-d5e094b270da.gif "Interactive shell demo")

# Supported types

- primitive types
- stl-like non-associative containers (std::vector, std::list...) that implement the push_back() method, parsed/serialized as JSON objects
- containers of containers

```
Sample std::vector<std::list<int>> :

[[0,2,4],[1,3,5],[]]

..can be used as a function parameter or return type.
```

# Dependencies
- GNU readline

# Todo
- support arguments of user-defined types
- support asynchronous execution of commands
- call functions remotely
