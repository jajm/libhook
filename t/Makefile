include ../config.mk

CPPFLAGS := -I../include
OBJS := $(patsubst %.c,%.o,$(wildcard *.c))
PROGRAMS := $(patsubst %.o,%,$(OBJS))
LIBRARY := ../src/$(PROJECT).la

.PHONY: clean

all: $(PROGRAMS)

$(PROGRAMS): % : %.o $(LIBRARY)
	$(LIBTOOL) --mode=link $(CC) -o $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(OBJS) $(PROGRAMS)
