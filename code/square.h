#ifndef __SQUARE_H__
#define __SQUARE_H__

#include "drawable.h"

class Square : public Drawable {
    public:
        //constructors
        Square(vec2 p1,vec2 p2,vec2 p3,vec2 p4,color3 color);

        //methods
        virtual void render();

        //destructor
        ~Square();
};

class FilledSquare : public Square {
    public:
        //constructors
        FilledSquare(vec2 p1,vec2 p2,vec2 p3,vec2 p4,color3 color)
            : Square(p1,p2,p3,p4,color) {

        }

        virtual void render();
};

#endif
