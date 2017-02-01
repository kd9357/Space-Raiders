#include "Projectile.h"

char WProjectile::getShape() const {
	return '!';
}

char uProjectile::getShape() const {
	return '*';
}

char PProjectile::getShape() const {
	return '|';
}
