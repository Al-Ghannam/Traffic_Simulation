#pragma once
#include "square.h"
#include "drawable.h"
//#include <vector>

class Kart
{
private:
	int m_id;
	float m_speed;
	vec2 points[4];
	float width,height;
	float offset;
	float lastUpdate;
	float deltaX;
	float deltaY;
	double kartCurveRadius;
	color3 m_color;
	bool shouldDelete,collided;
	//GLuint kartMatrixLocation;
public:
	Kart(float scale, float speed, char lane, color3 color, int id);
	//-------------------------------------------------------------------------
	int getId(){ return m_id; }
	//-------------------------------------------------------------------------
	Drawable* update(float dt);
	//-------------------------------------------------------------------------
	vec2 *getPoints(){ return points;}
	//-------------------------------------------------------------------------
	void Kart::applyTransform(mat3 kartMatrix);
	//-------------------------------------------------------------------------
	bool getDelete(){ return shouldDelete; }
	//-------------------------------------------------------------------------
	void setCollision(){ collided = true; }
	//-------------------------------------------------------------------------
	bool getCollision(){ return collided; }
	//-------------------------------------------------------------------------
	bool checkCollision(Kart* kart);
	//-------------------------------------------------------------------------
	Kart(void);
	~Kart(void);
};

