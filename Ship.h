#ifndef SHIP_H_BCDEFG
#define SHIP_H_BCDEFG

#include "GameObject.h"

class Ship : public GameObject{
public:
	Ship(int y, int x) : GameObject(y, x) {
		setY(y);
		setX(x);
	}
};

class EnemyShip : public Ship{
public:
	EnemyShip(int y, int x) : Ship(y, x) {
		setY(y);
		setX(x);
	}
};

class EnemyW : public EnemyShip{
public:
	EnemyW(int y, int x) : EnemyShip(y, x) {
		setY(y);
		setX(x);
	}
	char getShape() const;
};

class Enemyu : public EnemyShip{
public:
	Enemyu(int y, int x) : EnemyShip(y, x){
		setY(y);
		setX(x);
	}
	char getShape() const;
};
class PlayerShip : public Ship{
public:
	PlayerShip(int y, int x) : Ship(y, x) {
		setY(y);
		setX(x);
	}
	char getShape() const;
};

#endif
