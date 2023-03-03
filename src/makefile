CC = gcc 

PKGFLAGS = $(shell pkg-config --cflags gtk4) $(shell pkg-config --libs gtk4)

INCLUDES = -I/ -I/widgets

TARGET = app

WIDGETS = $(addprefix ./widgets/,\
	BoardButton\
)

all: build_app clean

build_app: compile_enum_types.c compile_resources compile_widgets compile_app
	${CC} $(addsuffix .o,$(notdir ${WIDGETS})) $(addsuffix .o,${TARGET}) gresource.o enum_types.o -o $(addsuffix .exe,${TARGET}) ${PKGFLAGS} ${INCLUDES}

compile_widgets:
	${CC} ${PKGFLAGS} ${INCLUDES} $(addsuffix .c,${WIDGETS}) -c

compile_app:
	${CC} ${PKGFLAGS} ${INCLUDES} $(addsuffix .c,${TARGET}) -c

compile_resources: make_resources
	${CC} ${PKGFLAGS} ${INCLUDES} gresource.c -c

make_resources:
	glib-compile-resources --generate-source gresource.xml

compile_enum_types.c: make_enum_types.c
	${CC} ${PKGFLAGS} ${INCLUDES} enum_types.c -c

make_enum_types.c: make_enum_types.h
	glib-mkenums --template=enum_types.c.in --output=enum_types.c $(addsuffix .h,${WIDGETS})

make_enum_types.h:
	glib-mkenums --template=enum_types.h.in --output=enum_types.h $(addsuffix .h,${WIDGETS})

clean:
	rm *.o; rm gresource.c