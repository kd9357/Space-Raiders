#ifndef GAMEOBJECT_H_ABCDEF
#define GAMEOBJECT_H_ABCDEF

class GameObject{
protected:
	int ypos;
	int xpos;
	int health;
public:
	GameObject(int y, int x) {
		setY(y);
		setX(x);
	}

	int getX() const;
	int getY() const;
	void setY(const int);
	void setX(const int);
	void setHealth(const int);
	void damage();
	int getHealth() const;
	void movXBy(const int);
	void movYBy(const int);
	bool isCollision(const GameObject&) const;
	virtual char getShape() const = 0;
};

#endif
