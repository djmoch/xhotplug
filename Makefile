include config.mk

all: xhotplug

.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -o $@ $<

xhotplug: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

install: xhotplug
	install -Dm755 xhotplug ${DESTDIR}${PREFIX}/bin/xhotplug

uninstall:
	-rm -f ${DESTDIR}${PREFIX}/bin/xhotplug

clean:
	-rm *.o xhotplug

.PHONY: all clean install uninstall
