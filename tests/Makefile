OUT		= tests
SRC		= ${wildcard *.cpp} ${filter-out ../main.cpp, ${wildcard ../*.cpp}}
OBJ		= ${SRC:.cpp=.o}
DEPENDS = .depends

CC		= g++
CFLAGS	:= ${CFLAGS} -g -O2 -Wall -Wextra -Wno-sign-compare $(shell pkg-config sdl SDL_gfx SDL_image SDL_ttf --cflags)
LDFLAGS := $(shell pkg-config sdl SDL_gfx SDL_image SDL_ttf --libs)

all: ${OUT}

${OUT}: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

.cpp.o:
	${CC} -c -o $@ $< ${CFLAGS}

${DEPENDS}: ${SRC}                                                                                   
	rm -f ./${DEPENDS}                                                                               
	${CC} ${CFLAGS} -MM $^ >> ./${DEPENDS}

depends: ${DEPENDS}

clean:
	${RM} ${OUT} ${OBJ}

-include ${DEPENDS}
.PHONY: all depends clean
