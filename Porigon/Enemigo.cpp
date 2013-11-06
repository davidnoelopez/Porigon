//
//  Enemigo.cpp
//  Porigon
//
//  Created by Dave on 10/22/13.
//  Copyright (c) 2013 Dave. All rights reserved.
//

#include "Enemigo.h"
#include <GLUT/glut.h>

Enemigo::Enemigo ()
{
    lado = rand()%4+1;
}

Enemigo::Enemigo ( float posx, float posy, int t)
{
    x = posx;
    y = posy;
    tipo = t;
    if (t == 1) {
        size = 20;
        vida = 1;
        velocidad = rand()%1+1;
    }
    else if (t == 2) {
        size = 20;
        vida = 2;
        velocidad = rand()%1+1;
    }
}

void Enemigo::dibuja(){
    if (tipo == 1) {
        glBegin(GL_POLYGON);
        glVertex2d(x-size/2, y-size/2);
        glVertex2d(x+size/2, y-size/2);
        glVertex2d(x, y+size/2);
        glEnd();
    }
    else if (tipo == 2) {
        glBegin(GL_POLYGON);
        glVertex2d(x-size/2, y-size/2);
        glVertex2d(x+size/2, y-size/2);
        glVertex2d(x+size/2, y+size/2);
        glVertex2d(x-size/2, y+size/2);
        glEnd();
    }
}