#include "GameObject.h"

int GameObject::getX() const{
	return xpos;
}

int GameObject::getY() const{
	return ypos;
}

void GameObject::setY(const int newY) {
	ypos = newY;
}

void GameObject::setX(const int newX) {
	xpos = newX;
}

void GameObject::setHealth(const int newHealth){
	health = newHealth;
}

void GameObject::damage() {
	health--;
}

int GameObject::getHealth() const {
	return health;
}


void GameObject::movXBy(const int newX) {
	xpos += newX;
}

void GameObject::movYBy(const int newY) {
	ypos += newY;
}

bool GameObject::isCollision(const GameObject& obj) const {
	return xpos == obj.getX() && ypos == obj.getY();
}
