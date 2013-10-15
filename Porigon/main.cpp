//
//  main.cpp
//  CuboSinNada
//  Created by Ma. Guadalupe Roque on 24/09/13.
//  Copyright (c) 2013 Ma. Guadalupe Roque. All rights reserved.
//
//
//  main.cpp
//  TransformacionesCuboGC13
//
//  Created by Maria  Roque on 21/02/13.
//  Copyright (c) 2013 Tecnológico de Monterrey. All rights reserved.
//

#include <GLUT/glut.h>
#include <iostream>
using namespace std;
const float medida = 25;  // Mitad del tamaño de cada lado del cubo
int angulo=0;
int anguloX=0;
int anguloY=0;
int anguloZ=0;
int explode=0;
int bullet=0;
int direction=0;
float xY, yY, xZ,yZ, x, y, xd, yd, xa, xb, yb, ya, xe, ye, es=20, trans=0.55;
int x1=0, x2=0,y1=0,y2=0;
int	screenWidth = 400, screenHeight = 400;



void dibujaDot(){
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    glPointSize( 20.0 );
    
    if(xd!=0 && yd!=0){
        xa=xa+(xd-xa)/10;
        ya=ya+(yd-ya)/10;
    }
    glBegin(GL_POINTS);
    glVertex3f(xa, ya, 0);
	glEnd();
    
}

//Se crea explocion
void dotExplode(){
    //se da el tamaño del punto
    glPointSize(es);
    if(es>=50){
        trans-=0.03;        //calcula transparencia
    }
    es+=0.5;                //calcula tamaño
    glColor4f( 1.0f, 1.0f, 1.0f, trans);
    glBegin(GL_POINTS);
    glVertex2d(xe, ye);
	glEnd();
    
    if(trans<=0){
        explode=0;
        es=20;
        trans=0.55;
    }
}

void shoot(){
    glColor3f( 1.0f, 1.0f, 1.0f );
    
    if(direction == 1){
        if (xb > 0-screenWidth/2) {
            glBegin(GL_LINES);
            glVertex2f( xb, yb);
            glVertex2f( xb-5, yb);
            xb-=screenWidth*.02;
            glEnd();
        }
        else {
            bullet = 0;
            direction = 0;
        }
    }
    
    glFlush();
}

void reshape (int a, int h)
{
    screenHeight = h;
    screenWidth = a;
    glViewport(0,0, a, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Sistema de coordenadas en 3D
    glOrtho(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -200, 200 ); //izq, der, abajo, arriba, cerca, lejos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    
}

void myMouse(int button, int state, int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && explode == 0){
            explode=1;
            xe=xa;
            ye=ya;
        }
    //    else if (button == GLUT_RIGHT_BUTTON){
    //        anguloZ+=10;
    //        x2=x;
    //        y2=y;
    //        dibuja();
    //    }
}

void myKeyboard(unsigned char key, int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
    switch (key)
    {
        case ' ':
            if ( explode == 0 ) {
                explode=1;
                xe=xa;
                ye=ya;
            }
            break;
        case 'a':
            if (bullet == 0) {
                direction=1;
                bullet=1;
                xb=xa;
                yb=ya;
                xe=xa;
                ye=ya;
            }
            break;
        case 'w':
            direction=2; break;
        case 'd':
            direction=3; break;
        case 's':
            direction=4; break;
    }
}

void myPasMouse(int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
    glColor3f( 1.0f, 1.0f, 1.0f );
    xd=x-screenWidth/2;
    yd=y-screenHeight/2;
    
}

void tecla (unsigned char t, int x, int y)
{
    
    if (t == 27 || t == 'q' || t=='Q') exit(0);
}

void time(int v)
{
    
    glutPostRedisplay();
    glutTimerFunc(20,time,1);
}

void dibuja() {
    glutSetCursor(GLUT_CURSOR_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPushMatrix();
    dibujaDot();
    //glPopMatrix();
    if(explode){
        //glPushMatrix();
        dotExplode();
        //glPopMatrix();
    }
    if (bullet) {
        shoot();
    }
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Porigon");
    // Para que OpenGl reconozca la profundidad en el eje Z
	glEnable(GL_DEPTH_TEST);
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glPointSize( 20.0 );
    glutTimerFunc(20,time,1);
    glutDisplayFunc(dibuja);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(tecla);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutPassiveMotionFunc(myPasMouse);
	glutMainLoop();
	return 0;
}