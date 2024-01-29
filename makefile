.SECONDEXPANSION:

ROOT_DIR := $(patsubst %/,%,$(dir $(realpath $(lastword $(MAKEFILE_LIST)))))

PKG_CONFIG ?= pkg-config

CC ?= gcc 

CPP ?= gcc -E

BUILD ?= debug
BUILD_DIR := build/${BUILD}
OBJ = ${BUILD_DIR}/obj
SRC = src
RES = res
INCLUDES = includes
INTERNAL = ${INCLUDES}/internal

# List BUILD directory + all subdirectories of SRC
SUBDIRS = ${BUILD_DIR} $(shell find ./${SRC} -type d -exec echo {} \; | sed 's|^./${SRC}|${OBJ}|')

RESOURCES = $(shell find ${RES} -type f)
HEADERS = $(shell find ${INCLUDES} -type f -name "*.h")
PUBLIC_HEADERS = ${filter-out ${INTERNAL}/%,${HEADERS}}
SOURCES = $(shell find ${SRC} -type f -name "*.c")
OBJECTS = $(subst ${SRC}/,${OBJ}/,$(patsubst %.c,%.o,$(SOURCES)))

cppflags.common = $(shell ${PKG_CONFIG} --cflags-only-I gtk4) -I./${INCLUDES}
cppflags.debug = -DDEBUG -DG_ENABLE_DEBUG
cppflags.release = -DRELEASE -DNDEBUG -UDEBUG -DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DGTK_NO_CHECK_CASTS
CPPFLAGS += ${cppflags.common} $(cppflags.$(BUILD))

cflags.common = -std=c17 -Werror -Wall -Wextra -Wundef -Wcast-align -Wstrict-overflow=5 -Wswitch-enum -Wconversion -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wno-unused-parameter
cflags.common += $(shell ${PKG_CONFIG} --cflags-only-other gtk4)
cflags.debug = -O0 -g3 -fsanitize=address -fsanitize=undefined
cflags.release = -O3
CFLAGS += ${cflags.common} $(cflags.$(BUILD))

ldflags.debug = -fsanitize=address -fsanitize=undefined
ldflags.release =
ldflags.common = $(shell ${PKG_CONFIG} --libs-only-L --libs-only-other gtk4)
LDFLAGS += ${ldflags.common} $(ldflags.$(BUILD)) 
LDLIBS += -lm $(shell ${PKG_CONFIG} --libs-only-l gtk4)


all: make_build_dir app

make_build_dir:
	mkdir -p ${SUBDIRS}

app: ${OBJ}/enum_types.o ${OBJ}/gresource.o ${OBJ}/marshals.o ${OBJECTS}
	${CC} ${LDFLAGS} $^ ${LDLIBS} -o ${BUILD_DIR}/app

${OBJ}/%.o: ${SRC}/%.c
	${CC} -c ${CFLAGS} ${CPPFLAGS} $< -o $@

generate_source: ${SRC}/gresource.c ${SRC}/enum_types.c ${INCLUDES}/enum_types.h ${SRC}/marshals.c ${SRC}/marshals.h

${SRC}/gresource.c: ${SRC}/gresource.xml ${RESOURCES}
	glib-compile-resources --generate-source ${SRC}/gresource.xml

${SRC}/enum_types.c: ${INCLUDES}/enum_types.h ${SRC}/enum_types.c.in 
	glib-mkenums --template=${SRC}/enum_types.c.in --output=${SRC}/enum_types.c $(filter-out ${INCLUDES}/enum_types.h,${PUBLIC_HEADERS})

${INCLUDES}/enum_types.h: ${SRC}/enum_types.h.in  $(filter-out ${INCLUDES}/enum_types.h,${PUBLIC_HEADERS})
	glib-mkenums --template=${SRC}/enum_types.h.in --output=${INCLUDES}/enum_types.h $(filter-out ${INCLUDES}/enum_types.h,${PUBLIC_HEADERS})

${SRC}/marshals.c: ${INCLUDES}/marshals.h ${SRC}/marshals.list
	glib-genmarshal --body --output=${SRC}/marshals.c --include-header=marshals.h ${SRC}/marshals.list

${INCLUDES}/marshals.h: ${SRC}/marshals.list
	glib-genmarshal --header --output=${INCLUDES}/marshals.h ${SRC}/marshals.list

clean:
	rm -rf build/*
	rm -f ${INCLUDES}/enum_types.h
	rm -f ${INCLUDES}/marshals.h
	rm -f ${SRC}/marshals.c
	rm -f ${SRC}/enum_types.c
	rm -f ${SRC}/gresource.c
