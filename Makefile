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
INCLUDE	 = -Iinclude -Ijsoncons/src
CFLAGS   = -std=c++11 -fPIC -Wall $(INCLUDE)

LIBS	 = -lreadline

LINKER   = g++ -o
LFLAGS   = -shared $(LIBS)

SOURCES := $(wildcard $(SRCDIR)/*)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS := $(SOURCES:$(SRCDIR)/%=$(OBJDIR)/%.o)
rm = rm -f

export

all: $(BINDIR)/$(TARGET) $(BINDIR)/example

$(BINDIR):
	@mkdir -p $(BINDIR)

$(OBJDIR):
	@mkdir -p $(OBJDIR)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR) 
	$(LINKER) $@ $(LFLAGS) $(OBJECTS)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/% | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR)/example: $(BINDIR)/$(TARGET)
	$(CC) -std=c++11 -Wl,-rpath=$(shell pwd)/$(BINDIR) $(INCLUDE) example/main.cpp -o $(BINDIR)/example -L$(BINDIR) -lshpp

install: $(BINDIR)/$(TARGET)
	mkdir -p $(INSTALL_INCLUDES)
	cp -r include/shpp/* $(INSTALL_INCLUDES)
	cp -r jsoncons/src/* $(INSTALL_INCLUDES)
	cp $(BINDIR)/$(TARGET) $(INSTALL_LIB)
	ldconfig

uninstall:
	rm -rf $(INSTALL_INCLUDES)
	rm -f $(INSTALL_LIB)/$(TARGET)

clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(BINDIR)
