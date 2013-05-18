#include "square.h"

Square::Square(vec2 p1,vec2 p2,vec2 p3,vec2 p4,color3 color) {
    pointsNum = 4;
	double scaleX = 900/2;
	double scaleY = 600/2;
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = vec2(p1.x/scaleX,p1.y/scaleY);
    points[1] = vec2(p2.x/scaleX,p2.y/scaleY);
    points[2] = vec2(p3.x/scaleX,p3.y/scaleY);
    points[3] = vec2(p4.x/scaleX,p4.y/scaleY);
    colors[0] = color;
    colors[1] = color;
    colors[2] = color;
    colors[3] = color;
    this->init();
}

void Square::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_LINE_LOOP , 0, pointsNum);
}

Square::~Square() {
    delete[] points;
    delete[] colors;
}

//-------------
// FILLED SQUARE CLASS

void FilledSquare::render() {
	vec2 a = points[0]; 
	vec2 b = points[1]; 
	vec2 c = points[2]; 
	vec2 d = points[3]; 
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, pointsNum);
}
