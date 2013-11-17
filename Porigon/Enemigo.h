//
//  Enemigo.h
//  Porigon
//
//  Created by Dave on 10/22/13.
//  Copyright (c) 2013 Dave. All rights reserved.
//

#ifndef __Porigon__Enemigo__
#define __Porigon__Enemigo__

#include <iostream>

class Enemigo
{
public:
    float x, y;
    int lado;
    int tipo;
    int vida;
    int size;
    int velocidad;
    int hit = 0;
    Enemigo ();
    Enemigo ( float posx, float posy, int t ); //constructor
    void dibuja(float xa, float ya);
};

#endif /* defined(__Porigon__Enemigo__) */
