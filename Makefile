ROOT=$(realpath $(CURDIR))

FINAL=$(ROOT)/as-z64

SOURCES=\
main.c\
encode/encode_operation.c\
options/get_options.c

INCLUDE=$(ROOT)/include
LIB=$(ROOT)/lib
export LIB
export INCLUDE

OBJECTS=$(SOURCES:.c=.o)
DEPENDENCIES=$(OBJECTS:.o=.d)

CC=gcc
CFLAGS?=-O2
CFLAGS+=-MD -Wall -Wno-switch
CPPFLAGS?=
CPPFLAGS+=-I$(INCLUDE)

LD=gcc
LDFLAGS?=
LDFLAGS+=-L$(LIB)
LDLIBS?=
LDLIBS+=$(patsubst %,-l%,$(PRELIBS))

PRETARGETS=

PRELIBS=parser

.PHONY: all targets libs build parser

all: libs targets build

targets: $(PRETARGETS)

libs: $(PRELIBS) 

build: $(OBJECTS) $(FINAL)

clean: 
	-rm -rf $(FINAL)
	-rm `find | grep "\.o"`
	-rm `find | grep "\.d"`
	-rm `find | grep "\.bin"`
	-rm -rf $(LIB)/*

parser:
	$(MAKE) -C parser build

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@ $(CPPFLAGS)

$(FINAL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(DEPENDENCIES)