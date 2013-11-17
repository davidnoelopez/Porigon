//
//  Enemigo.cpp
//  Porigon
//
//  Created by Dave on 10/22/13.
//  Copyright (c) 2013 Dave. All rights reserved.
//

#include "Enemigo.h"
#include <GLUT/glut.h>
#include "Imageloader.h"
#include <stdlib.h>
#include <string>

using namespace std;

static GLuint texturas[6];

Enemigo::Enemigo ()
{
    tipo = -1;
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

//Makes the image into a texture, and returns the id of the texture
void loadTexture(Image* image, int k){
    glBindTexture(GL_TEXTURE_2D, texturas[k]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
}

void Enemigo::dibuja(){
    if (&tipo != NULL) {
        string path = "/Users/Dave/Dropbox/Tareas Cloud/7º Semestre/Graficas/Porigon/Porigon/";
        string textura;
        switch (tipo) {
            case 1:
            {
                textura = path + "Textura1.bmp";
                Image* image = loadBMP(textura.c_str());
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2d(x-size/2, y-size/2);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2d(x+size/2, y-size/2);
                glTexCoord2f(0.5f, 1.0f);
                glVertex2d(x, y+size/2);
                glEnd();
                break;
            }
            case 2:
            {
                textura = path + "Textura1.bmp";
                Image* image = loadBMP(textura.c_str());
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glBegin(GL_QUADS);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(x-size/2, y-size/2,  1.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(x+size/2, y-size/2,  1.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(x+size/2, y+size/2,  1.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(x-size/2, y+size/2,  1.0f);
                glEnd();

                /*glBegin(GL_POLYGON);
                glVertex2d(x-size/2, y-size/2);
                glVertex2d(x+size/2, y-size/2);
                glVertex2d(x+size/2, y+size/2);
                glVertex2d(x-size/2, y+size/2);
                glEnd();*/
                break;
            }
            default:
                break;
        }
    }
}