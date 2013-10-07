//
//  main.cpp
//  CuboSinNada
//
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
int direction=0;
int bullets=0;
float xY, yY, xZ,yZ, x, y, xd, yd, xa, xb, yb, ya, xe, ye, es=20, trans=0.2;
int x1=0, x2=0,y1=0,y2=0;
int	screenWidth = 400, screenHeight = 400;

void time(int v)
{
    glutPostRedisplay();
    glutTimerFunc(10,time,1);
}

void dibujaDot(){
    
    if(xd!=0 && yd!=0){
        xa=xa+(xd-xa)/10;
        ya=ya+(yd-ya)/10;
    }
    glBegin(GL_POINTS);
    glVertex3f(xa, ya, 0);
	glEnd();
    
}

void dotExplode(){
    glPointSize(es);
    es+=0.8;
    if(es>=50){
        trans-=0.005;
    }
    glBegin(GL_POINTS);
    glColor4f( 1.0f, 1.0f, 1.0f, trans);
    glVertex3f(xe, ye, 0);
	glEnd();
    
    if(trans<=0){
        explode=0;
        es=20;
        trans=0.20;
    }
    
    glPointSize( 20.0 );
    glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
    
}

void shoot(){
    glColor3f( 1.0f, 1.0f, 1.0f );
    glBegin(GL_LINE);
    if(direction == 1 && xb > 0){
        glVertex2f( xb, yb);
        glVertex2f( xb-2, yb);
        xb-=5;
    }
    glEnd();
    glFlush();
}


void dibuja() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    dibujaDot();
    glPopMatrix();
    if(explode==1){
        glPushMatrix();
        dotExplode();
        glPopMatrix();
    }
    shoot();
    glutSwapBuffers();
}


void reshape (int a, int h)
{
    glViewport(0,0, a, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Sistema de coordenadas en 3D
    glOrtho(-200.0, 200, -200, 200, -200, 200 ); //izq, der, abajo, arriba, cerca, lejos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    
}

void myMouse(int button, int state, int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
    //    if (button == GLUT_LEFT_BUTTON){
    //        explode=1;
    //        xe=xa;
    //        ye=ya;
    //    }
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
            explode=1; xe=xa; ye=ya; break;
        case 'a':
            direction=1; explode=1; xb=xa; yb=ya; xe=xa; ye=ya; break;
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
    xd=x-200;
    yd=y-200;
    
}

void tecla (unsigned char t, int x, int y)
{
    
    if (t == 27 || t == 'q' || t=='Q') exit(0);
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Cubo de colores ");
    // Para que OpenGl reconozca la profundidad en el eje Z
	glEnable(GL_DEPTH_TEST);
    glEnable( GL_POINT_SMOOTH );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glPointSize( 20.0 );
    glutTimerFunc(10,time,1);
    glutDisplayFunc(dibuja);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(tecla);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutPassiveMotionFunc(myPasMouse);
	glutMainLoop();
	return 0;
}