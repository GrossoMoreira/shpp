# ------------------------------------------------
# Generic Makefile
#
# Author: yanick.rochon@gmail.com
# Date  : 2011-08-10
#
#                thanks to http://stackoverflow.com/users/128940/beta
# ------------------------------------------------

TARGET   = libshpp.so

INSTALL_INCLUDES = /usr/local/include/shpp
INSTALL_LIB = /usr/local/lib

CC       = g++
INCLUDE	 = -I. -Icpp-readline/src
CFLAGS   = -std=c++11 -fPIC -Wall -Werror $(INCLUDE)

LIBS	 = -lreadline

LINKER   = g++ -o
LFLAGS   = -shared $(LIBS)

SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

READLINE = $(OBJDIR)/cpp-readline.o

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
	$(CC) $(CFLAGS) -c cpp-readline/src/Console.cpp -o $(READLINE) $(LIBS)

$(BINDIR)/$(TARGET): $(OBJECTS) bindir
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
