PKG_CONFIG ?= pkg-config

CC ?= gcc 

CPP ?= gcc -E

CPPFLAGS := ${CPPFLAGS} -DDEBUG

CFLAGS := $(CFLAGS) -std=c17 -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes 

INCLUDES = $(shell ${PKG_CONFIG} --cflags-only-I gtk4) -I/ -I/widgets

LDLIBS := ${LDLIBS} -lm

LDFLAGS := ${LDFLAGS}

LDLIBS += $(shell ${PKG_CONFIG} --libs-only-l gtk4)

CFLAGS += $(shell ${PKG_CONFIG} --cflags-only-other gtk4) ${INCLUDES}

LDFLAGS += $(shell ${PKG_CONFIG} --libs-only-L --libs-only-other gtk4)

RESOURCES = $(shell echo $(shell find ui -type f))

WIDGETS = MainMenuWindow \
	CheckersBoardButton \
	CheckersWindow \
	CheckersGameOver \
	CheckersBoard \
	CheckersPlayerSelect


OBJ = obj

SRC = src

BUILD = build/debug

app: ${OBJ}/app.o $(addprefix ${OBJ}/widgets/,$(addsuffix .o,${WIDGETS})) ${OBJ}/enum_types.o ${OBJ}/gresource.o
	${CC} ${LDFLAGS} $^ ${LDLIBS} -o ${BUILD}/app

${OBJ}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} ${CPPFLAGS} $< -o $@

${SRC}/gresource.c: ${SRC}/gresource.xml ${RESOURCES}
	glib-compile-resources --generate-source ${SRC}/gresource.xml

${SRC}/enum_types.c: ${SRC}/enum_types.h ${SRC}/enum_types.c.in
	glib-mkenums --template=enum_types.c.in --output=${SRC}/enum_types.c $(addprefix ${SRC}/widgets/,$(addsuffix .h,${WIDGETS}))

${SRC}/enum_types.h: ${SRC}/enum_types.h.in
	glib-mkenums --template=${SRC}/enum_types.h.in --output=${SRC}/enum_types.h $(addprefix ${SRC}/widgets/,$(addsuffix .h,${WIDGETS}))

clean:
	find . -name "*.o" -print0 | xargs -0 rm ; rm -rf ${BUILD} ; mkdir ${BUILD}
