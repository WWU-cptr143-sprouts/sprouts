OUT		= sprouts
SRC		= ${wildcard *.cpp}
OBJ		= ${SRC:.cpp=.o}
DEPENDS = .depends

CC		= g++
CFLAGS	:= -g -std=c++11 -O2 -Wall -Wextra -Wpedantic -ISDL/include/SDL ${CFLAGS}
LDFLAGS := -LSDL/lib -lSDLmain -lSDL.dll -lSDL -lSDL_gfx_linux -lSDL_ttf -lSDL_image  SDL/lib/SDL_image.lib SDL/lib/SDL_ttf.lib

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

include ${DEPENDS}
.PHONY: all depends clean
