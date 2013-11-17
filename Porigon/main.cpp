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
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

//clases custom
#include "Bala.h"
#include "Enemigo.h"
#include "imageloader.h"

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
int puntos = 0;
float dificultad = 1, aumento = 100;
bool boss = false;

//  Initialize light source and shading model (GL_FLAT).

float mat_specular [] = {0.0,0.0,0.0,1.0};
float mat_emission [] = {0.0,0.0,0.0,1.0};
float mat_ambient_diffuse [] = {0.5,0.5,0.5,1.0};
float mat_shininess = 0.4*128;
float spot_dir [] = {0.0,0.0,-1.0};
float spot_cutoff = 170.0;
float spot_exponent = 0.0;
float light_ambient [] = {0.5,0.5,0.5,1.0};
float light_diffuse_specular [] = {0.8,0.8,0.8,1.0};
float light_pos [] = {0.0,0.0,3.0, 1.0};
float colorDOT [] = {1.0,1.0,1.0,1.0};


void dibujaDot(){
    //cambia el color del DOT si es golpeado
    if (hit) {
        colorDOT[0] = 1.0;
        colorDOT[1] = 0.0;
        colorDOT[2] = 0.0;
        hit--;
    }
    else{
        colorDOT[0] = 1.0;
        colorDOT[1] = 1.0;
        colorDOT[2] = 1.0;
    }
    
    //movimiento fluido
    if(xd!=0 && yd!=0){
        xa=xa+(xd-xa)/10;
        ya=ya+(yd-ya)/10;
    }
    
    //dibuja DOT
    glPushMatrix ();
    glTranslatef(xa, ya, 0);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv(colorDOT);
    glutSolidSphere(10, 50, 50);
    glColor4fv(mat_emission);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix ();
}

void escribirTexto(std::string texto, double x, double y, void * font)
{
    float focus_emission [] = {1.0,1.0,1.0,1.0};
    glRasterPos2f(x, y);
    for (std::string::iterator i = texto.begin(); i != texto.end(); ++i){
        //pone color
        glColorMaterial(GL_FRONT,GL_EMISSION);
        glEnable(GL_COLOR_MATERIAL);
        glColor4fv(focus_emission);
        glutBitmapCharacter(font, *i);
    }
}

void pintarMarcador()
{
    //aumenta dificultad dependiendo de puntaje
    if (puntos > aumento) {
        dificultad += .1;
        aumento += aumento/2;
    }
    
    //agrega cubo gigante boss
    if (puntos%500 == 0 && puntos > 0) {
        if (boss) {
            Enemigo *auxenemigo = new Enemigo((rand()%screenWidth/2)-screenWidth, (rand()%screenHeight)-screenHeight/2, 10);
            vectorEnemigos.push_back(auxenemigo);
            boss = false;
        }
    }
    else {
        boss = true;
    }
        
    stringstream puntaje;
    puntaje << puntos;
    glPushMatrix();
    
    escribirTexto("Score: " + puntaje.str(), -screenWidth/2+screenWidth/25, screenHeight/2-screenHeight/25, GLUT_BITMAP_HELVETICA_18);
    glPopMatrix();
    
    /*if (0) {
        texto = "GAME OVER.";
    }*/
    
    glFlush();
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
    if (bullet > 0)
        for (int index = 0; index < sizeof(arregloBalas)/sizeof(*arregloBalas); index++) {
            if (arregloBalas[index].viva) {
                float focus_emission [] = {1.0,0.0,0.1,1.0};
                glPushMatrix();
                //pone color
                glColorMaterial(GL_FRONT,GL_EMISSION);
                glEnable(GL_COLOR_MATERIAL);
                glColor4fv(focus_emission);
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
                glPopMatrix();
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
        Bala auxBala = Bala(xa, ya, dir);
        while (continuaWhile) {
            if (arregloBalas[auxbullet].viva == 0) {
                arregloBalas[auxbullet] = auxBala;
                continuaWhile = 0;
            }
            else if (auxbullet < sizeof(arregloBalas)/sizeof(*arregloBalas)-1)
                auxbullet++;
            else
                auxbullet = 0;
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
    glOrtho(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -screenHeight, screenHeight); //izq, der, abajo, arriba, cerca, lejos
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
        case 'A':
        case 'a':
            crearBala(1); break;
        case 'W':
        case 'w':
            crearBala(2); break;
        case 'D':
        case 'd':
            crearBala(3); break;
        case 'S':
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
    int cantidadEnemigos = 10*dificultad;
    if (vectorEnemigos.size() < cantidadEnemigos) {
        int tipoRandom = rand()%100+1;
        if (tipoRandom < 50) {
            tipoRandom = 1;
        }
        else if (tipoRandom < 80){
            tipoRandom = 2;
        }
        else if (tipoRandom <= 100){
            tipoRandom = 3;
        }
        Enemigo *auxenemigo = new Enemigo();
        //lado izq
        if (auxenemigo->lado == 1) {
            auxenemigo = new Enemigo((rand()%screenWidth/2)-screenWidth, (rand()%screenHeight)-screenHeight/2, tipoRandom);
            vectorEnemigos.push_back(auxenemigo);
        }
        //lado arriba
        else if (auxenemigo->lado == 2) {
            auxenemigo = new Enemigo((rand()%screenWidth)-screenWidth/2, (rand()%screenHeight/2)-screenHeight, tipoRandom);
            vectorEnemigos.push_back(auxenemigo);
        }
        //lado der
        else if (auxenemigo->lado == 3) {
            auxenemigo = new Enemigo((rand()%screenWidth)+screenWidth/2, (rand()%screenHeight)-screenHeight/2, tipoRandom);
            vectorEnemigos.push_back(auxenemigo);
        }
        //lado abajo
        else if (auxenemigo->lado == 4) {
            auxenemigo = new Enemigo((rand()%screenWidth)-screenWidth/2, (rand()%screenHeight)+screenHeight/2, tipoRandom);
            vectorEnemigos.push_back(auxenemigo);
        }
        
    }
    
    glutPostRedisplay();
    glutTimerFunc(500, crearEnemigos, 1);
}

int revisarColisionBalas(int vec)
{
    int result = 0;
    int tamanio = sizeof(arregloBalas)/sizeof(*arregloBalas);
    if (bullet)
        for (int count = 0; count < tamanio; count++) {
            if (arregloBalas[count].viva) {
                if ((arregloBalas[count].x < (vectorEnemigos.at(vec)->x + vectorEnemigos.at(vec)->size/2)) && (arregloBalas[count].x > (vectorEnemigos.at(vec)->x - vectorEnemigos.at(vec)->size/2)) &&
                    (arregloBalas[count].y < (vectorEnemigos.at(vec)->y + vectorEnemigos.at(vec)->size/2)) && (arregloBalas[count].y > (vectorEnemigos.at(vec)->y - vectorEnemigos.at(vec)->size/2))) {
                    bullet--;
                    arregloBalas[count].viva = 0;
                    vectorEnemigos.at(vec)->vida--;
                    vectorEnemigos.at(vec)->hit = 5;
                    result = 1;
                }
            }
        }
    return result;
}

void dibujarEnemigos()
{
    int tam = 0;
    if (!vectorEnemigos.empty())
        tam = (int)vectorEnemigos.size();
    
    for (int n = 0; n < tam; n++){
        if (&vectorEnemigos.at(n)->lado != nullptr) {
            glColor3f(1, 1, 1);
            
            vectorEnemigos.at(n)->dibuja(xa, ya);
            
            int colicionBalas = revisarColisionBalas(n);
            
            //cambia posicion de enemigo y revisa que no choque con el DOT
            if ((((vectorEnemigos.at(n)->x + vectorEnemigos.at(n)->size/2) < xa-10 || (vectorEnemigos.at(n)->x - vectorEnemigos.at(n)->size/2) > xa+10) ||
                ((vectorEnemigos.at(n)->y + vectorEnemigos.at(n)->size/2) < ya-10 || (vectorEnemigos.at(n)->y - vectorEnemigos.at(n)->size/2) > ya+10)) &&
                vectorEnemigos.at(n)->vida > 0) {
                
                if (xa-7 > vectorEnemigos.at(n)->x){
                    if(((xa-vectorEnemigos.at(n)->x)/200)>vectorEnemigos.at(n)->velocidad)
                        vectorEnemigos.at(n)->x += (vectorEnemigos.at(n)->velocidad*((xa-vectorEnemigos.at(n)->x)/200))*dificultad;
                    else
                        vectorEnemigos.at(n)->x +=vectorEnemigos.at(n)->velocidad*dificultad;
                }
                else if (xa+7 < vectorEnemigos.at(n)->x){
                    if(((xa-vectorEnemigos.at(n)->x)/200)>vectorEnemigos.at(n)->velocidad)
                        vectorEnemigos.at(n)->x += (vectorEnemigos.at(n)->velocidad*((xa-vectorEnemigos.at(n)->x)/200))*dificultad;
                    else
                        vectorEnemigos.at(n)->x -=vectorEnemigos.at(n)->velocidad*dificultad;
                }
                if (ya-7 > vectorEnemigos.at(n)->y){
                    if(((ya-vectorEnemigos.at(n)->y)/200)>vectorEnemigos.at(n)->velocidad)
                        vectorEnemigos.at(n)->y += (vectorEnemigos.at(n)->velocidad*((ya-vectorEnemigos.at(n)->y)/200))*dificultad;
                    else
                        vectorEnemigos.at(n)->y +=vectorEnemigos.at(n)->velocidad*dificultad;
                }
                else if (ya+7 < vectorEnemigos.at(n)->y){
                    if(((ya-vectorEnemigos.at(n)->y)/200)>vectorEnemigos.at(n)->velocidad)
                        vectorEnemigos.at(n)->y += (vectorEnemigos.at(n)->velocidad*((ya-vectorEnemigos.at(n)->y)/200))*dificultad;
                    else
                        vectorEnemigos.at(n)->y -=vectorEnemigos.at(n)->velocidad*dificultad;
                }
            }
            //Enemigo coliciona con DOT o con bala
            else {
                
                if (colicionBalas == 0) {
                    hit = 5;
                    puntos -= 100;
                    vectorEnemigos.erase(vectorEnemigos.begin()+n);
                    tam--;
                    n--;
                }
                else if(colicionBalas == 1 && vectorEnemigos.at(n)->vida == 0){
                    
                    puntos += vectorEnemigos.at(n)->tipo*5;
                    vectorEnemigos.erase(vectorEnemigos.begin()+n);
                    tam--;
                    n--;
                }
                
            }
        }
    }
}

//Grid inferior
void crearGrid(){
    float colorGrid [] = {1.0,1.0,1.0,1.0};
    if (hit) {
        colorGrid[0] = 1.0;
        colorGrid[1] = 0.0;
        colorGrid[2] = 0.0;
        hit--;
    }
    else{
        colorGrid[0] = 0.1;
        colorGrid[1] = 0.1;
        colorGrid[2] = 0.1;
    }
    glPushMatrix();
    glTranslatef(0, 0, -screenHeight);
    //rotacion inicial
    glRotated(90, 0, 0, 0);
    //rotacion paralax
    glRotated(1, xa, ya, 0);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv(colorGrid);
    glutWireSphere(screenWidth, screenWidth/10, screenHeight/15);
    glPopMatrix();
}

void time(int v)
{
    
    glutPostRedisplay();
    glutTimerFunc(20,time,1);
}

void mostrarLuz()
{
    /*light_pos[0] = xa;
    light_pos[1] = ya;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spot_dir);
    glTranslatef(light_pos[0],light_pos[1],light_pos[2]);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv(mat_emission);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
     */
}

void display()
{
    glutSetCursor(GLUT_CURSOR_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    crearGrid();
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
    pintarMarcador();
    mostrarLuz();
    glutSwapBuffers();
}

void init(){
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse_specular);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_diffuse_specular);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,spot_cutoff);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,spot_exponent);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
    glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
    glEnable(GL_LIGHT0);
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
    glEnable(GL_TEXTURE_2D);

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
    glEnable(GL_LINE_SMOOTH);
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    init();
    glPointSize( 20.0 );
    glutTimerFunc(20,time,1);
    glutTimerFunc(500, crearEnemigos, 1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(tecla);
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutPassiveMotionFunc(myPasMouse);
	glutMainLoop();
	return 0;
}