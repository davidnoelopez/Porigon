//
//  David Lopez A01138388
//  Patricio Gonzalez A01190487
//
//  main.cpp
//  Porigon
//
//

#include <GLUT/glut.h>
#include <iostream>
#include <vector>
#include "Bala.h"

using namespace std;
const float medida = 25;  // Mitad del tamaño de cada lado del cubo
int angulo=0;
int anguloX=0;
int anguloY=0;
int anguloZ=0;
int explode=0;
int bullet=0;
float xY, yY, xZ,yZ, x, y, xd, yd, xa, xb, yb, ya, xe, ye, es=20, trans=0.55;
int x1=0, x2=0,y1=0,y2=0;
int	screenWidth = 800, screenHeight = 800;
Bala arregloBalas[10];
Bala bul;



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
    glVertex3f(xe, ye, -1);
	glEnd();
    
    if(trans<=0){
        explode=0;
        es=20;
        trans=0.55;
    }
}

void shoot(){
    glColor3f( 1.0f, 1.0f, 1.0f );
    
        
    for (int index = 0; index < sizeof(arregloBalas); index++) {
        switch (arregloBalas[index].direccion) {
            case 1:
                if (arregloBalas[index].x > -screenWidth/2) {
                    glBegin(GL_LINES);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y);
                    glVertex2f( arregloBalas[index].x+10, arregloBalas[index].y);
                    arregloBalas[index].x-=screenWidth*.01;
                    glEnd();
                    
                }
                else if (arregloBalas[index].viva) {
                    arregloBalas[index].viva = 0;
                    bullet--;
                }
                break;
                
            case 2:
                if (arregloBalas[index].y < screenHeight/2) {
                    glBegin(GL_LINES);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y-10);
                    arregloBalas[index].y+=screenHeight*.01;
                    glEnd();
                    
                }
                else if (arregloBalas[index].viva) {
                    arregloBalas[index].viva = 0;
                    bullet--;
                }
                break;
                
            case 3:
                if (arregloBalas[index].x < screenWidth/2) {
                    glBegin(GL_LINES);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y);
                    glVertex2f( arregloBalas[index].x-10, arregloBalas[index].y);
                    arregloBalas[index].x+=screenWidth*.01;
                    glEnd();
                    
                }
                else if (arregloBalas[index].viva) {
                    arregloBalas[index].viva = 0;
                    bullet--;
                }
                break;
                
            case 4:
                if (arregloBalas[index].y > -screenHeight/2) {
                    glBegin(GL_LINES);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y);
                    glVertex2f( arregloBalas[index].x, arregloBalas[index].y+10);
                    arregloBalas[index].y-=screenHeight*.01;
                    glEnd();
                    
                }
                else if (arregloBalas[index].viva) {
                    arregloBalas[index].viva = 0;
                    bullet--;
                }
                break;
                
            default:
                break;
        }
    }
    
    glFlush();
}

void crearBala (int dir) {
    if (bullet < 10) {
        bullet++;
        int auxbullet = bullet;
        int continuaWhile = 1;
        while (continuaWhile) {
            if (&arregloBalas[auxbullet] == NULL || arregloBalas[auxbullet].viva == 0) {
                arregloBalas[auxbullet] = Bala(xa, ya, dir);
                continuaWhile = 0;
            }
            else
                auxbullet++;
        }
    }
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
            
        }
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
            crearBala(1); break;
        case 'w':
            crearBala(2); break;
        case 'd':
            crearBala(3); break;
        case 's':
            crearBala(4); break;
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
    dibujaDot();
    if(explode){
        dotExplode();
    }
    if (bullet > 0) {
        shoot();
    }
    glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
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