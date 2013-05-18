#include "Kart.h"

#define DEFAULTH 10
#define DEFAULTW 20
#define MID -237.5
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif
Kart::Kart(void)
{
}

Kart::Kart(float scale, float speed, char lane, color3 color, int id){
	m_id = id;
	m_speed = speed;
	offset = (lane == 'r')? 15 : -15;
	m_color = color;
	width = scale*DEFAULTW;
	height = scale* DEFAULTH;
	vec2 a,b,c,d;
	a = vec2(450,(offset+MID)); b = vec2(a.x + width,a.y);
	c = vec2(a.x,a.y+height); d = vec2(b.x,b.y+height);
	points[0] = a;
	points[1] = b;
	points[2] = c;
	points[3] = d;
	kartCurveRadius = -(offset+MID+height/2);
	shouldDelete = false;
	collided = false;
}

Drawable* Kart::update(float dt){
	
	// Update poisition:
		//Check for collision:
	if(!collided){
		if(points[0].x >= -140){
			vec2 kartMatrix;
			deltaX = dt*m_speed;
			deltaX = (points[0].y < 0)? -deltaX : deltaX;
			kartMatrix[0] = deltaX;
			kartMatrix[1] = 0.0f;
			for(int i=0; i<4;i++){
				points[i] = points[i] + kartMatrix;
			}
		}
		else
		{
			// Translate to below origin:
			deltaX = 140;
			mat3 kartMatrix;
			kartMatrix[0][0] = 1.0f; kartMatrix[0][1] = 0.0f; kartMatrix[0][2] = deltaX;;
			kartMatrix[1][0] = 0.0f; kartMatrix[1][1] = 1.0f; kartMatrix[1][2] = 0.0f;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);

			// Rotate around origin:
			// Using w = v/r & Angle = w * t (Angular motion equations)
			double w = m_speed / kartCurveRadius;
			double angle = w * dt;
			kartMatrix[0][0] = cosf(angle); kartMatrix[0][1] = sinf(angle); kartMatrix[0][2] = 0.0f;
			kartMatrix[1][0] = -sinf(angle); kartMatrix[1][1] = cosf(angle); kartMatrix[1][2] = 0.0f;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);

			//Translate to original position:
			kartMatrix[0][0] = 1.0f; kartMatrix[0][1] = 0.0f; kartMatrix[0][2] = -deltaX;;
			kartMatrix[1][0] = 0.0f; kartMatrix[1][1] = 1.0f; kartMatrix[1][2] = 0.0f;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);
		}
	
		if(points[3].x > 450 + width + 1){
			shouldDelete = true;
		}
	}
	else{
		static float collisionRot = 0.0;
		float delta = 0.01;
		if(collisionRot > 2*M_PI)
			shouldDelete = true;
		else{
			// Translate to origin:
			deltaX = (points[0].x + width/2);
			deltaY = (points[0].y + height/2);
			mat3 kartMatrix;
			kartMatrix[0][0] = 1.0f; kartMatrix[0][1] = 0.0f; kartMatrix[0][2] = deltaX;
			kartMatrix[1][0] = 0.0f; kartMatrix[1][1] = 1.0f; kartMatrix[1][2] = deltaY;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);

			// Rotate around origin:
			collisionRot += delta;
			kartMatrix[0][0] = cos(collisionRot); kartMatrix[0][1] = -sin(collisionRot); kartMatrix[0][2] = 0.0f;
			kartMatrix[1][0] = sin(collisionRot); kartMatrix[1][1] = cos(collisionRot); kartMatrix[1][2] = 0.0f;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);

			//Translate to original position:
			kartMatrix[0][0] = 1.0f; kartMatrix[0][1] = 0.0f; kartMatrix[0][2] = -deltaX;;
			kartMatrix[1][0] = 0.0f; kartMatrix[1][1] = 1.0f; kartMatrix[1][2] = -deltaY;
			kartMatrix[2][0] = 0.0f; kartMatrix[2][1] = 0.0f; kartMatrix[2][2] = 1.0f;
			applyTransform(kartMatrix);
		}
	}

	Drawable* kart = new FilledSquare(points[0],points[1],points[2],points[3],m_color);
	return kart;
}
//----------------------------------------------------------------------------
bool between(float a, float b, float c){
	return ((a>=b && a<=c) || (a<=b && a>=c))? true : false;
}
//-----------------------------------------------------------------------------
bool Kart::checkCollision(Kart *kart){
	float rightA,leftA,bottomA,topA,
		rightB,leftB,bottomB,topB;
	//Sides of objectA:
	rightA = points[0].x + width;
	leftA  = points[0].x;
	topA = points[0].y;
	bottomA = points[0].y + height;
	//Sides of objectB:
	rightB = kart->points[0].x + width;
	leftB  = kart->points[0].x;
	topB = kart->points[0].y;
	bottomB = kart->points[0].y + height;
	//Basic idea: check if a side in one object is between two sides in the other object.
	if((between(rightA,rightB,leftB) || between(leftA,rightB,leftB)) &&
			(between(topA,topB,bottomB) || between(bottomA,topB,bottomB)))
				return true;
	else
		return false;
}
//-----------------------------------------------------------------------------
void Kart::applyTransform(mat3 kartMatrix){
	vec3 temp;
	for(int i=0; i<4; i++){
		temp = kartMatrix * vec3(points[i],1.0);
		points[i].x = temp.x;
		points[i].y = temp.y;
	}
}
Kart::~Kart(void)
{
}
