#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "drawable.h"
class Circle : public Drawable {
    public:
        //constructors
        // np = number of points to use in drawing the cirlce
        // the more the better
        Circle(vec2 center,GLfloat radius,int np,color3 color);

        //methods
        virtual void render();

        //destructor
        ~Circle();
};

class FilledCircle : public Drawable {
    public:

        //constructors
        FilledCircle (vec2 center,GLfloat radius,int np,color3 color);

        //methods
        virtual void render();

        //constructor
        ~FilledCircle();
};

#endif
