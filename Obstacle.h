#ifndef OBSTACLE_H_DEFGHI
#define OBSTACLE_H_DEFGHI

#include "GameObject.h"

class Obstacle : public GameObject {
public:
 Obstacle(int y, int x) : GameObject(y, x) {
    setY(y);
    setX(x);
    setHealth(5);
  }
  char getShape() const;
};

#endif
