PREFIX := /usr/local
MANPATH := ${PREFIX}/share/man
X11BASE := /usr/X11R6

SRC = xhotplug.c
OBJ = ${SRC:.c=.o}
INCS = -I${X11BASE}/include
LIBS = -L${X11BASE}/lib -lxcb -lxcb-randr

CPPFLAGS := -DVERSION=\"0.1.0dev0\"
CFLAGS := -std=c99 -pedantic-errors -Wall -Wextra -Werror -O2 -c ${INCS}
LDFLAGS := ${LIBS}
