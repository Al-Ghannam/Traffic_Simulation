#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "include/Angel.h"

typedef vec3 color3;
class Drawable {
    public:
        static GLuint shaderProgram;
        int pointsNum;
        vec2* points;
        vec3* colors;

        //methods
        virtual void render();
		void applyTransform(mat3 m);
		void update();
        //destructor
        ~Drawable();

    protected:
        GLuint vao,buffer;
        void init();
};

#endif
