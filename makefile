PKG_CONFIG ?= pkg-config

CC ?= gcc 

CPP ?= gcc -E

TARGET ?= debug

SRC = src
BUILD = build/${TARGET}
OBJ = ${BUILD}/obj

# List all subdirectories of SRC
SUBDIRS = build ${BUILD} $(shell find ./${SRC} -type d -exec echo {} \; | sed 's|^./${SRC}|${OBJ}|')

CPPFLAGS := ${CPPFLAGS} -D$(shell echo "$(TARGET)" | tr 'a-z' 'A-Z')

INCLUDES = $(shell ${PKG_CONFIG} --cflags-only-I gtk4) -I/ -I/widgets

CFLAGS := $(CFLAGS) -std=c17 -Werror -Wall -Wextra -Wundef -Wcast-align -Wstrict-overflow=5 -Wswitch-enum -Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wno-unused-parameter
CFLAGS += $(shell ${PKG_CONFIG} --cflags-only-other gtk4) ${INCLUDES}

LDLIBS := ${LDLIBS} -lm
LDLIBS += $(shell ${PKG_CONFIG} --libs-only-l gtk4)

LDFLAGS := ${LDFLAGS}
LDFLAGS += $(shell ${PKG_CONFIG} --libs-only-L --libs-only-other gtk4)

RESOURCES = $(shell echo $(shell find resources -type f))

WIDGETS = MainMenuWindow \
	CheckersBoardButton \
	CheckersWindow \
	CheckersGameOver \
	CheckersBoard 


all: make_build_dir app

make_build_dir:
	mkdir -p ${SUBDIRS}

app: ${OBJ}/enum_types.o ${OBJ}/app.o $(addprefix ${OBJ}/widgets/,$(addsuffix .o,${WIDGETS})) ${OBJ}/gresource.o
	${CC} ${LDFLAGS} $^ ${LDLIBS} -o ${BUILD}/app

${OBJ}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} ${CPPFLAGS} $< -o $@

${SRC}/gresource.c: ${SRC}/gresource.xml ${RESOURCES}
	glib-compile-resources --generate-source ${SRC}/gresource.xml

${SRC}/enum_types.c: ${SRC}/enum_types.h ${SRC}/enum_types.c.in
	glib-mkenums --template=${SRC}/enum_types.c.in --output=${SRC}/enum_types.c $(addprefix ${SRC}/widgets/,$(addsuffix .h,${WIDGETS}))

${SRC}/enum_types.h: ${SRC}/enum_types.h.in
	glib-mkenums --template=${SRC}/enum_types.h.in --output=${SRC}/enum_types.h $(addprefix ${SRC}/widgets/,$(addsuffix .h,${WIDGETS}))

clean:
	rm -rf build
	rm ${SRC}/enum_types.h
	rm ${SRC}/enum_types.c
