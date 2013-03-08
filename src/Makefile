include ../config.mk

CPPFLAGS := -I../include
LIBTOOL_VERSION := $(LIBTOOL_CURRENT):$(LIBTOOL_REVISION):$(LIBTOOL_AGE)
OBJS := $(patsubst %.c,%.lo,$(wildcard *.c))
HEADERS := $(wildcard ../include/*.h *.h)
LIBRARY := $(PROJECT).la
LDFLAGS := `pkg-config --libs libgends lll`

.PHONY: clean

all: $(LIBRARY)

$(LIBRARY): $(OBJS)
	$(LIBTOOL) --mode=link $(CC) $(LDFLAGS) -rpath $(LIBDIR) -version-info $(LIBTOOL_VERSION) -o $@ $^

%.lo: %.c $(HEADERS)
	$(LIBTOOL) --mode=compile $(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

install: $(LIBRARY)
	$(LIBTOOL) --mode=install $(INSTALL) $< $(LIBDIR)

clean:
	rm -rf *.o *.lo .libs $(LIBRARY)
