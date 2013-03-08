include config.mk

SUBDIRS = src include t

.PHONY: lib test $(SUBDIRS)

all: lib test

$(SUBDIRS):
	$(MAKE) -C $@

t: src
lib: src
test: t

clean:
	for DIR in $(SUBDIRS); do \
		$(MAKE) -C $$DIR clean; \
	done

install:
	for DIR in $(SUBDIRS); do \
		$(MAKE) -C $$DIR install; \
	done
	$(INSTALL) --mode=0644 $(PROJECT).pc $(PKGCONFIGDIR)
