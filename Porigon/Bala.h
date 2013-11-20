//
//  Bala.h
//  Porigon
//
//  Created by Dave on 10/15/13.
//  Copyright (c) 2013 Dave. All rights reserved.
//

#ifndef __Porigon__Bala__
#define __Porigon__Bala__

#include <iostream>
#include "Sound.h"
#include "Sound.h"
class Bala
{
public:
    float x, y;
    int direccion;
    int viva;
    Sound *sonido;
    Bala ();
    Bala ( float posx, float posy, int dir ); //constructor
};

#endif /* defined(__Porigon__Bala__) */
