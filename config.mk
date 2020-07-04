PREFIX := /usr/local
MANPATH := ${PREFIX}/share/man
X11BASE := /usr/X11R6

SRC = xhotplug.c
DIST_SRC = ${SRC} Makefile config.mk xhotplug.1
OBJ = ${SRC:.c=.o}
INCS = -I${X11BASE}/include
LIBS = -L${X11BASE}/lib -lxcb -lxcb-randr
VERSION = 0.1.0

CPPFLAGS := -DVERSION=\"${VERSION}\"
CFLAGS := -std=c99 -pedantic-errors -Wall -Wextra -Werror -O2 -c ${INCS}
LDFLAGS := ${LIBS}
