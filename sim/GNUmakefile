name := HodoSim
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

COMMON_DIR = $(HOME)/common
LIB_DIR = $(HOME)/lib
EXTRA_INCLUDES = -I./ -I./include -I$(COMMON_DIR)

.PHONY: all
all: dict lib bin

dict:	include/RootLinkDef.h include/HodoEvent.hh
	rootcint -f src/Dict.cc -c -Iinclude -O2 -g -Wall -W -fPIC -pipe -Woverloaded-virtual -m32 HodoEvent.hh RootLinkDef.h

include $(G4INSTALL)/config/architecture.gmk

CPPFLAGS += $(shell $(ROOTSYS)/bin/root-config --cflags) $(EXTRA_INCLUDES)
LDFLAGS  += $(shell $(ROOTSYS)/bin/root-config --libs)
CPPFLAGS  += -Wl,--no-as-needed
LDFLAGS  += -Wl,--no-as-needed
CPPFLAGS  += -Woverloaded-virtual
LDFLAGS  += -Woverloaded-virtual

EXTRALIBS = -L$(LIB_DIR) -lCommandLineInterface

test:	
	echo $(LDFLAGS)

include $(G4INSTALL)/config/binmake.gmk
