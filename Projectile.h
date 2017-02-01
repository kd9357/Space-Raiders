#ifndef PROJECTILE_H_CDEFGH
#define PROJECTILE_H_CDEFGH

#include "GameObject.h"

class Projectile : public GameObject{
public:
	Projectile(int y, int x) : GameObject(y, x) {
		setY(y);
		setX(x);
	}
};

class WProjectile : public Projectile{
public:
	WProjectile(int y, int x) : Projectile(y, x) {
		setY(y);
		setX(x);
	}
	char getShape() const;
};

class uProjectile : public Projectile{
public:
	uProjectile(int y, int x) : Projectile(y, x){
		setY(y);
		setX(x);
	}
	char getShape() const;
};

class PProjectile : public Projectile{
public:
	PProjectile(int y, int x) : Projectile(y, x) {
		setY(y);
		setX(x);
	}
	char getShape() const;
};

#endif
