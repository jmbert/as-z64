ROOT=$(realpath $(CURDIR))

FINAL=$(LIB)/libparser.a


SOURCES=\
parse.c\
tokenise.c\
tokenise_helpers.c\
parse_args.c

OBJECTS=$(SOURCES:.c=.o)
DEPENDENCIES=$(OBJECTS:.o=.d)

INCLUDEPARSER=$(ROOT)/include

CC=gcc
CFLAGS?=-O2
CFLAGS+=-MD -L$(LIB) -Wall -Wno-unused-value -Wno-parentheses -Wno-address
CPPFLAGS?= 
CPPFLAGS+= -I$(INCLUDE) -I$(INCLUDEPARSER)

LD=ar
LDFLAGS?=
LDFLAGS+=rcs
LDLIBS?=
LDLIBS+=

.PHONY: build

build: $(GRAMMAR) $(OBJECTS) $(FINAL)

%.o:%.c
	$(CC) -c $(CFLAGS) $< -o $@ $(CPPFLAGS)

$(FINAL): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(DEPENDENCIES)