CC = gcc 

CFLAGS =  $(shell pkg-config --cflags gtk4) $(shell pkg-config --libs gtk4) -lm -no-pie -Wall -Wshadow \
-Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -std=c17 -DDEBUG 

INCLUDES = -I/ -I/widgets

TARGET = app

WIDGETS = $(addprefix ./widgets/,\
	MainMenuWindow\
	CheckersBoardButton\
	CheckersWindow\
	CheckersGameOver\
	CheckersBoard\
	CheckersPlayerSelect\
)

all: build_app clean

build_app: compile_enum_types.c compile_resources compile_widgets compile_app
		${CC} $(addsuffix .o,$(notdir ${WIDGETS})) $(addsuffix .o,${TARGET}) gresource.o enum_types.o -o ${TARGET} ${CFLAGS} ${INCLUDES}

compile_widgets:
	${CC} ${CFLAGS} ${INCLUDES} $(addsuffix .c,${WIDGETS}) -c

compile_app:
	${CC} ${CFLAGS} ${INCLUDES} $(addsuffix .c,${TARGET}) -c

compile_resources: make_resources
	${CC} ${CFLAGS} ${INCLUDES} gresource.c -c

make_resources:
	glib-compile-resources --generate-source gresource.xml

compile_enum_types.c: make_enum_types.c
	${CC} ${CFLAGS} ${INCLUDES} enum_types.c -c

make_enum_types.c: make_enum_types.h
	glib-mkenums --template=enum_types.c.in --output=enum_types.c $(addsuffix .h,${WIDGETS})

make_enum_types.h:
	glib-mkenums --template=enum_types.h.in --output=enum_types.h $(addsuffix .h,${WIDGETS})

clean:
	rm *.o; rm gresource.c
