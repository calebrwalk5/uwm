# uwm - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = drw.c uwm.c util.c
OBJ = ${SRC:.c=.o}

all: options uwm

options:
	@echo uwm build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

uwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f uwm ${OBJ} uwm-${VERSION}.tar.gz

dist: clean
	mkdir -p uwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		uwm.1 drw.h util.h ${SRC} uwm.png transient.c uwm-${VERSION}
	tar -cf uwm-${VERSION}.tar uwm-${VERSION}
	gzip uwm-${VERSION}.tar
	rm -rf uwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f uwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/uwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < uwm.1 > ${DESTDIR}${MANPREFIX}/man1/uwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/uwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/uwm\
		${DESTDIR}${MANPREFIX}/man1/uwm.1

.PHONY: all options clean dist install uninstall
