.EXPORT_ALL_VARIABLES:

.PHONY: clean all

LIB_DIR = $(HOME)/lib

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --glibs)
ROOTINC      := -I$(shell root-config --incdir)

COMMON_DIR = $(HOME)/common/

ALLIBS  = -lm $(ROOTLIBS) $(ROOTGLIBS) -L$(LIB_DIR) -lCommandLineInterface -lHodoEvent

CPP             = g++
#CFLAGS		= -fPIC -O2 -pipe -Wall -W -Woverloaded-virtual $(ROOTCFLAGS)
CFLAGS		= -pedantic -Wall -Wno-long-long -g -O3 $(ROOTCFLAGS) -fPIC
DFLAGS		= -Wall -Wno-long-long -g -O3 $(ROOTCFLAGS) -fPIC
CFLAGS		+= -Wl,--no-as-needed
DFLAGS		+= -Wl,--no-as-needed

INCLUDES        = -I./ -I$(COMMON_DIR)
LFLAGS		= -fPIC -O2 -g $(ROOTLDFLAGS)
LIBS 		= $(ALLIBS)

all: HodoEvent HodoAna
	echo Done

HodoAna: HodoAna.cc 
	$(CPP) $(CFLAGS) $(INCLUDES) $^ $(LIBS) -o $@
	cp HodoAna $(HOME)/bin

HodoEvent: HodoEvent.o HodoEventDictionary.o 
	$(CPP) $(LFLAGS) -shared -Wl,-soname,libHodoEvent.so -o $(LIB_DIR)/libHodoEvent.so $^ -lc

%.o: %.cc %.hh
	@echo Default .o rule
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIB_DIR)/libCommandLineInterface.so:
	@cd $(COMMON_DIR); make 

HodoEventDictionary.o: HodoEventDictionary.cc HodoEventDictionary.h
	 $(CPP) -p -fPIC $(DFLAGS) -c $<

HodoEventDictionary.cc: HodoEvent.hh HodoEventLinkDef.h 
	 rm -f HodoEventDictionary.cc HodoEventDictionary.h; rootcint -f $@ -c $(SWITCH) HodoEvent.hh HodoEventLinkDef.h 


clean:
	rm  -f HodoAna *.o *Dictionary*
