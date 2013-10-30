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
#include "Enemigo.h"
#include <math.h>
#define _USE_MATH_DEFINES

using namespace std;
const float medida = 25;  // Mitad del tamaño de cada lado del cubo
int angulo=0;
int anguloX=0;
int anguloY=0;
int anguloZ=0;
int explode=0;
int bullet=0;
float xY, yY, xZ,yZ, x, y, xd, yd, xa, xb, yb, ya, xe, ye, es=20, trans=0.55;
int x1=0, x2=0;
int	screenWidth = 800, screenHeight = 800;
float carga = M_PI*2;
int maxEnemigos = 10;
int hit = 0;
Bala arregloBalas[10];
Bala bul;
vector<Enemigo *> vectorEnemigos;

void dibujaDot(){
    if (hit) {
        glColor4f( 1.0f, 0.0f, 0.0f, 1.0f );
        hit--;
    }
    else
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

//Se crea arco
void dibujaArco(float cx, float cy, float r, float start_angle, float arc_angle, int num_segments)
{
    float theta = arc_angle / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    
    float tangetial_factor = tanf(theta);
    
    float radial_factor = cosf(theta);
    
    
    float xr = r * cosf(start_angle);//we now start at the start angle
    float yr = r * sinf(start_angle);
    glColor4f( 0.5f, 0.5f, 0.5f, 1.0f );
    glBegin(GL_LINE_STRIP);//since the arc is not a closed curve, this is a strip now
    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(xr + cx, yr + cy);
        
        float tx = -yr;
        float ty = xr;
        
        xr += tx * tangetial_factor;
        yr += ty * tangetial_factor;
        
        xr *= radial_factor;
        yr *= radial_factor;
    } 
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
    carga+=.01265822785*M_PI*2;
    
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

void moverBalas(){
    
    
        
    for (int index = 0; index < sizeof(arregloBalas); index++) {
        if (arregloBalas[index].viva) {
            glColor3f( 1.0f, 1.0f, 1.0f );
        }
        else
            glColor3f( 0.0f, 0.0f, 0.0f );
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

void crearBala (int dir)
{
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
                carga = 0;
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

void crearEnemigos(int v)
{
    if (vectorEnemigos.size() < 10) {
        
        Enemigo *auxenemigo = new Enemigo();
        //lado izq
        if (auxenemigo->lado == 1) {
            auxenemigo = new Enemigo((rand()%screenWidth/2)-screenWidth, (rand()%screenHeight)-screenHeight/2, 1);
        }
        //lado arriba
        else if (auxenemigo->lado == 2) {
            auxenemigo = new Enemigo((rand()%screenWidth)-screenWidth/2, (rand()%screenHeight/2)-screenHeight, 1);
        }
        //lado der
        else if (auxenemigo->lado == 3) {
            auxenemigo = new Enemigo((rand()%screenWidth)+screenWidth/2, (rand()%screenHeight)-screenHeight/2, 1);
        }
        //lado abajo
        else if (auxenemigo->lado == 4) {
            auxenemigo = new Enemigo((rand()%screenWidth)-screenWidth/2, (rand()%screenHeight)+screenHeight/2, 1);
        }
        vectorEnemigos.push_back(auxenemigo);
    }
    
    glutPostRedisplay();
    glutTimerFunc(500, crearEnemigos, 1);
}

void revisarColisionBalas(int vec)
{
    for (int count = 0; count < sizeof(arregloBalas); count++) {
        if (!(((vectorEnemigos.at(vec)->x + vectorEnemigos.at(vec)->size/2) < arregloBalas[count].x || (vectorEnemigos.at(vec)->x - vectorEnemigos.at(vec)->size/2) > arregloBalas[count].x) ||
            ((vectorEnemigos.at(vec)->y + vectorEnemigos.at(vec)->size/2) < arregloBalas[count].y || (vectorEnemigos.at(vec)->y - vectorEnemigos.at(vec)->size/2) > arregloBalas[count].y)) && arregloBalas[count].viva) {
            bullet--;
            arregloBalas[count].viva = 0;
            if (vectorEnemigos.at(vec)->vida == 1) {
                vectorEnemigos.erase(vectorEnemigos.begin()+vec);
            }
            else{
                vectorEnemigos.at(vec)->vida--;
            }
            
        }
    }
}

void dibujarEnemigos()
{
    for (int n = 0; n < vectorEnemigos.size(); n++) {
        glColor3f(1, 1, 1);
        vectorEnemigos.at(n)->dibuja();
        
        revisarColisionBalas(n);
        
        if (((vectorEnemigos.at(n)->x + vectorEnemigos.at(n)->size/2) < xa-10 || (vectorEnemigos.at(n)->x - vectorEnemigos.at(n)->size/2) > xa+10) ||
            ((vectorEnemigos.at(n)->y + vectorEnemigos.at(n)->size/2) < ya-10 || (vectorEnemigos.at(n)->y - vectorEnemigos.at(n)->size/2) > ya+10)) {
            if (xa > vectorEnemigos.at(n)->x){
                if(((xa-vectorEnemigos.at(n)->x)/200)>vectorEnemigos.at(n)->velocidad)
                    vectorEnemigos.at(n)->x += (vectorEnemigos.at(n)->velocidad*((xa-vectorEnemigos.at(n)->x)/200));
                else
                    vectorEnemigos.at(n)->x +=vectorEnemigos.at(n)->velocidad;
            }
            else if (xa < vectorEnemigos.at(n)->x){
                if(((xa-vectorEnemigos.at(n)->x)/200)>vectorEnemigos.at(n)->velocidad)
                    vectorEnemigos.at(n)->x += (vectorEnemigos.at(n)->velocidad*((xa-vectorEnemigos.at(n)->x)/200));
                else
                    vectorEnemigos.at(n)->x -=vectorEnemigos.at(n)->velocidad;
            }
            if (ya > vectorEnemigos.at(n)->y){
                if(((ya-vectorEnemigos.at(n)->y)/200)>vectorEnemigos.at(n)->velocidad)
                    vectorEnemigos.at(n)->y += (vectorEnemigos.at(n)->velocidad*((ya-vectorEnemigos.at(n)->y)/200));
                else
                    vectorEnemigos.at(n)->y +=vectorEnemigos.at(n)->velocidad;
            }
            else if (ya < vectorEnemigos.at(n)->y){
                if(((ya-vectorEnemigos.at(n)->y)/200)>vectorEnemigos.at(n)->velocidad)
                    vectorEnemigos.at(n)->y += (vectorEnemigos.at(n)->velocidad*((ya-vectorEnemigos.at(n)->y)/200));
                else
                    vectorEnemigos.at(n)->y -=vectorEnemigos.at(n)->velocidad;
            }
        }
        else {
            vectorEnemigos.erase(vectorEnemigos.begin()+n);
            hit = 5;
        }
    }
}

void time(int v)
{
    
    glutPostRedisplay();
    glutTimerFunc(20,time,1);
}

void dibuja()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(2);
    dibujaDot();
    dibujaArco(xa, ya, 13, M_PI/2, carga, 100);
    if(explode){
        dotExplode();
    }
    if (bullet > 0) {
        moverBalas();
    }
    dibujarEnemigos();
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
    glutTimerFunc(500, crearEnemigos, 1);
    glutDisplayFunc(dibuja);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(tecla);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutPassiveMotionFunc(myPasMouse);
	glutMainLoop();
	return 0;
}