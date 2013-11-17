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
#include <math.h>

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
    switch (tipo) {
        case 1:
        {
            size = 20;
            vida = 1;
            velocidad = rand()%1+1;
            break;
        }
        case 2:
        {
            size = 20;
            vida = 2;
            velocidad = rand()%2+1;
            break;
        }
        case 3:
        {
            size = 25;
            vida = 4;
            velocidad = rand()%2+2;
            break;
        }
        case 10:
        {
            size = 100;
            vida = 50;
            velocidad = 3;
            break;
        }
        default:
            break;
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

void Enemigo::dibuja(float xa, float ya){
    if (&tipo != NULL) {
        string path = "";
        string textura;
        switch (tipo) {
                //dibuja triangulo
            case 1:
            {
                float focus_emission [] = {0.0,0.0,1.0,1.0};
                textura = path + "Textura1.bmp";
                Image* image = loadBMP(textura.c_str());
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                double angulo = atan2((ya - y), (xa - x)) * 180 / M_PI;
                glPushMatrix();
                glTranslatef(x, y, 0);
                glRotated(angulo-90, 0, 0, 1);
                //pone color
                glColorMaterial(GL_FRONT,GL_EMISSION);
                glEnable(GL_COLOR_MATERIAL);
                glColor4fv(focus_emission);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.0f);
                glVertex2d(-size/2, -size/2);
                glTexCoord2f(1.0f, 0.0f);
                glVertex2d(size/2, -size/2);
                glTexCoord2f(0.5f, 1.0f);
                glVertex2d(0, size/2);
                glEnd();
                glPopMatrix();
                break;
            }
                //dibuja cuadro
            case 2:
            {
                float focus_emission[] = {0.0,1.0,0.0,1.0};
                if (hit) {
                    focus_emission[0] = 1.0;
                    focus_emission[1] = 0.0;
                    focus_emission[2] = 1.0;
                    hit--;
                }
                else {
                    focus_emission[0] = 0.0;
                    focus_emission[1] = 1.0;
                    focus_emission[2] = 0.0;
                }
                
                textura = path + "Textura1.bmp";
                Image* image = loadBMP(textura.c_str());
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                
                double angulo = atan2((ya - y), (xa - x)) * 180 / M_PI;
                glPushMatrix();
                glTranslatef(x, y, 0);
                glRotated(angulo-90, 0, 0, 1);
                //pone color
                glColorMaterial(GL_FRONT,GL_EMISSION);
                glEnable(GL_COLOR_MATERIAL);
                glColor4fv(focus_emission);
                glBegin(GL_QUADS);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(-size/2, -size/2,  1.0f);
                glTexCoord2f(1.0f, 0.0f);
                glVertex3f(size/2, -size/2,  1.0f);
                glTexCoord2f(1.0f, 1.0f);
                glVertex3f(size/2, size/2,  1.0f);
                glTexCoord2f(0.0f, 1.0f);
                glVertex3f(-size/2, size/2,  1.0f);
                glEnd();
                glPopMatrix();
                
                break;
            }
                //dibuja pentagono
            case 3:
            {
                float focus_emission [] = {0.0,1.0,1.0,1.0};
                if (hit) {
                    focus_emission[0] = 1.0;
                    focus_emission[1] = 0.0;
                    focus_emission[2] = 0.0;
                    hit--;
                }
                else {
                    focus_emission[0] = 0.0;
                    focus_emission[1] = 1.0;
                    focus_emission[2] = 1.0;
                }
                textura = path + "Textura1.bmp";
                Image* image = loadBMP(textura.c_str());
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                
                double angulo = atan2((ya - y), (xa - x)) * 180 / M_PI;
                glPushMatrix();
                glTranslatef(x, y, 0);
                glRotated(angulo-90, 0, 0, 1);
                //pone color
                glColorMaterial(GL_FRONT,GL_EMISSION);
                glEnable(GL_COLOR_MATERIAL);
                glColor4fv(focus_emission);
                glBegin(GL_POLYGON);
                glTexCoord2f(0.0f, 0.5f);
                glVertex3f(-size/2, 0,  1.0f);
                glTexCoord2f(0.25f, 0.0f);
                glVertex3f(-size/4, -size/2,  1.0f);
                glTexCoord2f(0.75f, 0.0f);
                glVertex3f((size/4), -size/2,  1.0f);
                glTexCoord2f(1.0f, 0.5f);
                glVertex3f(size/2, 0,  1.0f);
                glTexCoord2f(0.5f, 1.0f);
                glVertex3f(0, size/2,  1.0f);
                glEnd();
                glPopMatrix();
                
                break;
            }
                //dibuja cubo grande
            case 10:
            {
                float focus_emission [] = {1.0,0.0,1.0,1.0};
                if (hit) {
                    focus_emission[0] = 0.5;
                    focus_emission[1] = 1.0;
                    focus_emission[2] = 0.5;
                    hit--;
                }
                else {
                    focus_emission[0] = 1.0;
                    focus_emission[1] = 0.0;
                    focus_emission[2] = 1.0;
                }
                double angulo = atan2((ya - y), (xa - x)) * 180 / M_PI;
                
                glPushMatrix();
                glTranslatef(x, y, 0);
                glRotated(angulo, x, y, 1);
                //pone color
                glColorMaterial(GL_FRONT,GL_EMISSION);
                glEnable(GL_COLOR_MATERIAL);
                glColor4fv(focus_emission);
                glutSolidCube(size);
                glPopMatrix();
                break;
            }
            default:
                break;
        }
    }
}