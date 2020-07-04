# See LICENSE file for copyright and license details
.POSIX:

include config.mk

all: xhotplug

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ $<

xhotplug: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: xhotplug
	install -Dm755 xhotplug ${DESTDIR}${PREFIX}/bin/xhotplug
	install -Dm644 xhotplug.1 ${DESTDIR}${MANPATH}/man1/xhotplug.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/xhotplug
	rm -f ${DESTDIR}${MANPATH}/man1/xhotplug.1

clean:
	rm -f *.o xhotplug

dist:
	rm -rf xhotplug-${VERSION}
	mkdir xhotplug-${VERSION}
	cp ${DIST_SRC} xhotplug-${VERSION}
	tar -cf - xhotplug-${VERSION} | gzip > xhotplug-${VERSION}.tar.gz

distclean:
	rm -rf xhotplug-${VERSION}
	rm -f xhotplug-${VERSION}.tar.gz

.PHONY: all clean install uninstall dist distclean
