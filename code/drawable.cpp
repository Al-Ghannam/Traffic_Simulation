#include "drawable.h"

GLuint Drawable::shaderProgram = 0;

void Drawable::render() {} 

void Drawable::init() {
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // BUFFERS
    // Create and initialize a buffer object
    // NOTE: since both vertices positions and colors are static data
    // (ie. have the same use) we can put them on the same buffer
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec2) + sizeof(vec3)) * pointsNum, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * pointsNum, sizeof(vec3) * pointsNum, colors);

    // ATTRIBUTES
    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( shaderProgram, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(0));

    // Initialize the vertex color attribute from the vertex shader
    GLuint loc2 = glGetAttribLocation( shaderProgram, "vColor" );
    glEnableVertexAttribArray( loc2 );
    glVertexAttribPointer( loc2, 3, GL_FLOAT, GL_FALSE, 0,
            BUFFER_OFFSET(sizeof(vec2) * pointsNum) );
}

Drawable::~Drawable (){
    // delete vertex array object
    glDeleteVertexArrays(1,&vao);

    // delete buffers
    glDeleteBuffers(1,&buffer);

    // disable position and color attrib arays
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    // unbind vao and buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void Drawable::applyTransform(mat3 m){
	vec3 temp;
	for(int i=0; i<pointsNum; i++){
		temp = m * vec3(points[i],1.0);
		points[i].x = temp.x;
		points[i].y = temp.y;
	}
	update();
}
void Drawable::update() {
    glBindVertexArray( vao );
    glBufferData( GL_ARRAY_BUFFER, (sizeof(vec2) + sizeof(vec3)) * pointsNum, NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec2) * pointsNum, points);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec2) * pointsNum, sizeof(vec3) * pointsNum, colors);
}