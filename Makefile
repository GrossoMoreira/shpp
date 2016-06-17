# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

TARGET   = libshpp.so

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

INSTALL_INCLUDES = /usr/local/include/shpp
INSTALL_LIB = /usr/local/lib

CC       = g++
INCLUDE	 = -I. -Icpp-readline/src
CFLAGS   = -std=c++11 -fPIC -Wall -Werror $(INCLUDE)

LIBS	 = -lreadline

LINKER   = g++ -o
LFLAGS   = -shared $(LIBS)

READLINE = $(OBJDIR)/libcpp-readline.a

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.h) cpp-readline/src
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm = rm -f

export

all: $(BINDIR)/$(TARGET) $(BINDIR)/example
	@date

bindir:
	@mkdir -p $(BINDIR)

objdir:
	@mkdir -p $(OBJDIR)

$(READLINE): objdir
	$(MAKE) -C cpp-readline
	@cp cpp-readline/libcpp-readline.a $(READLINE)

$(BINDIR)/$(TARGET): $(OBJECTS) $(READLINE) bindir
	$(LINKER) $@ $(LFLAGS) $(OBJECTS) $(READLINE)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp $(READLINE) objdir
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/example: $(BINDIR)/$(TARGET)
	$(CC) -std=c++11 -L$(BINDIR) -I. example/main.cpp -o $(BINDIR)/example -lshpp

install: $(BINDIR)/$(TARGET)
	mkdir $(INSTALL_INCLUDES)
	cp -r include/* $(INSTALL_INCLUDES)
	cp $(BINDIR)/$(TARGET) $(INSTALL_LIB)
	ldconfig

uninstall:
	rm -rf $(INSTALL_INCLUDES)
	rm -f $(INSTALL_LIB)/$(TARGET)

clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)
