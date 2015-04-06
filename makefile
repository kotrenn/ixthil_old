OBJS = main.o common.o geometry.o surface.o resourcemanager.o\
       inputmanager.o background.o menu.o options.o pausemenu.o\
       level.o actor.o ship.o bullet.o item.o player.o\
       enemies.o dark.o quickship.o teleportership.o ballos.o\
       bigship.o boss5.o boss6.o slideship.o orbitship.o\
       colorship.o surroundship.o spinship.o stateship.o\
       credits.o
FLAGS = -O3 -Wall -g
LINKS = -lm -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
#LINKS = -lm -lmingw32 -lSDLmain -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf

ixthil.exe: ${OBJS}
	g++ -o ixthil ${OBJS} ${FLAGS} ${LINKS}

.cpp.o: $<
	-g++ ${FLAGS} -c -o $@ $<