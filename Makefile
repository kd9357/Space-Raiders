CC = g++ -g -O0

all: main.o
	${CC} GameObject.cpp Ship.cpp Projectile.cpp Obstacle.cpp  main.cpp -lncurses
clean:
	rm *.o
