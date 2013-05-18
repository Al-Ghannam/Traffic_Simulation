
#include "circle.h"

Circle::Circle(vec2 center,GLfloat radius,int np,color3 color) {
    pointsNum = np; //np 
    points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    for (int i=0;i<np;i++){
        double angle = 180.0/(np-1) * i;
        double rad = angle / 180.0 * 3.1415;
        points[i] = center + vec2(radius*cos(rad),radius*sin(rad));
        colors[i] = color;
    }
    this->init();
}

void Circle::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_SUN_mesh_array, 0, pointsNum);
}

Circle::~Circle() {
    delete[] points;
    delete[] colors;
}

//-----------
// FILLED CIRCLE

//constructors
FilledCircle::FilledCircle (vec2 center,GLfloat radius,int np,color3 color) {
    pointsNum = np + 1; //np + center point
	double scaleX = 900/2;
	double scaleY = 600/2;
	center = vec2(center.x/scaleX, center.y/scaleY);
	points = new vec2[pointsNum];
    colors = new color3[pointsNum];
    points[0] = center;
    colors[0] = color;
    for (int i=0;i<np;i++){
        double angle = 360.0/(np-1) * i;
        double rad = angle / 180.0 * 3.1415;
        points[i+1] = center + vec2(radius*cos(rad)/scaleX,radius*sin(rad)/scaleY);
        colors[i+1] = color;
    }
    this->init();
}

void FilledCircle::render() {
    glBindVertexArray( vao );
    glDrawArrays( GL_TRIANGLE_FAN, 0, pointsNum);
}

FilledCircle::~FilledCircle() {
    delete[] points;
    delete[] colors;
}
