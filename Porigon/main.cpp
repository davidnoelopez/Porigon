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

/* variables explosion */
int explode=0;
float xe, ye;
float expRadio = 0, expGrosor, trans, maxDist;

//Cantidad de balas
int bullet=0;

//variables para posicion de DOT
float x, y, xd, yd, xa, ya;

//tamaño inicial de pantalla
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
int vidas = 0;
int menu = 1;

GLuint backgroundMenu;
GLuint texturasMain[6];

//  variables para inicializar luces

float mat_specular [] = {0.0,0.0,0.0,1.0};
float mat_emission [] = {0.0,0.0,0.0,1.0};
float mat_ambient_diffuse [] = {0.5,0.5,0.5,1.0};
float mat_shininess = 0.4*128;
float spot_dir [] = {0.0,0.0,-1.0};

//para luz0
float spot_cutoff = 30;
float spot_exponent = 1.0;
float light_ambient [] = {0.5,0.5,0.5,1.0};
float light_diffuse_specular [] = {0.8,0.8,0.8,1.0};
float light_pos [] = {0.0,0.0,3.0, 1.0};


//para luz1 sobre DOT pero Afecta grid
float spot_cutoff1 = 30.0;
float spot_exponent1 = 1.0;
float light_ambient1 [] = {0.2,0.2,0.2,1.0};
float light_diffuse_specular1 [] = {0.8,0.8,0.8,1.0};
float light_posDOT [] = {0.0,0.0,-900.0, 1.0};
float colorDOT [] = {1.0,1.0,1.0,1.0};


void escribirTexto(std::string texto, double x, double y, void * font)
{
    //se deshabilita luz para poner color
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(x, y);
    
    for (std::string::iterator i = texto.begin(); i != texto.end(); ++i){
        glutBitmapCharacter(font, *i);
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void pintarVidas() {
    glEnable(GL_COLOR_MATERIAL);
    float vidasY = screenHeight/2-(screenHeight/25)*1.7;
    float vidasX = -screenWidth/2+screenWidth/25;
    escribirTexto("Lives: ", vidasX, vidasY, GLUT_BITMAP_HELVETICA_18);
    for (int i = 0; i < vidas; i++) {
        glPushMatrix();
        glTranslatef(vidasX+60+15*i, vidasY+5, 0);
        glutSolidSphere(5, 20, 20);
        glPopMatrix();
    }
    glDisable(GL_COLOR_MATERIAL);
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
    
    
    escribirTexto("Score: " + puntaje.str(), -screenWidth/2+screenWidth/25, screenHeight/2-screenHeight/25, GLUT_BITMAP_HELVETICA_18);
    
    glFlush();
}

//Se crea arco
void dibujaArco(float cx, float cy, float r, float start_angle, int num_segments)
{
    float theta = carga / float(num_segments - 1);//theta is now calculated from the arc angle instead, the - 1 bit comes from the fact that the arc is open
    
    float tangetial_factor = tanf(theta);
    
    float radial_factor = cosf(theta);
    
    
    float xr = r * cosf(start_angle);//we now start at the start angle
    float yr = r * sinf(start_angle);
    if (carga >= M_PI*2) {
        glColor4f(0.075, 0.008, 0.673, 1.0f);
        glPushMatrix();
        glTranslatef(xa, ya, 1);
        glutSolidTorus(1, r, 50, 50);
        glPopMatrix();
        
    }
    else {
        glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );
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
    if (expRadio > 0) {
        glColor4f( 0.075, 0.008, 0.673, 0 );
        glPushMatrix();
        glTranslatef(xe, ye, 1);
        glutSolidTorus(expGrosor, expRadio, 50, 50);
        glPopMatrix();
    }
}

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
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix ();
    glTranslatef(xa, ya, 0);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glutSolidSphere(10, 50, 50);
    glColor4fv(mat_emission);
    glPopMatrix ();
    dibujaArco(xa, ya, 13, M_PI/2, 100);
    glDisable(GL_COLOR_MATERIAL);
    
    //cambia posicion de la luz
    if (expRadio == 0) {
        light_posDOT[0] = xa;
        light_posDOT[1] = ya;
    }
    if (expRadio == 0 && light_posDOT[2] > -900) {
        light_posDOT[2]-=12;
    }
}

//timer para generar carga de arco
void cargaArc(int state) {
    if (carga < M_PI*2)
    {
        carga+=.001*M_PI*2;
        glutTimerFunc(70, cargaArc, 1);
    }
    else {
        explode = 0;
    }
}

//timer para explosion del arco
void arcExplode(int state) {
    glColor4f( 0.5f, 0.5f, 0.5f, 1.0f );
    switch (state) {
        //empieza explosion
        case 1:{
            carga = 0;
            xe=xa;
            ye=ya;
            explode=1;
            expRadio = 13;
            expGrosor = 1;
            expRadio++;
            trans = 1;
            maxDist = 0;
            light_posDOT[0]=xe;
            light_posDOT[1]=ye;
            //revisa la distancia mas grande a alguna de las esquinas
            for (int i = 0; i < 4; i++) {
                float xAux = 0, yAux = 0;
                switch (i) {
                    case 0:
                        xAux = -screenWidth/2;
                        yAux = screenHeight/2;
                        break;
                    case 2:
                        xAux = -screenWidth/2;
                        yAux = -screenHeight/2;
                        break;
                    case 3:
                        xAux = screenWidth/2;
                        yAux = -screenHeight/2;
                        break;
                    case 4:
                        xAux = screenWidth/2;
                        yAux = screenHeight/2;
                        break;
                    default:
                        break;
                }
                if (maxDist<sqrtf( powf((xAux-xe), 2)+powf((yAux-ye), 2) )) {
                    maxDist = sqrtf( powf((xAux-xe), 2)+powf((yAux-ye), 2) );
                }
            }
            
            glutTimerFunc(70, cargaArc, 1);
            glutPostRedisplay();
            glutTimerFunc(1, arcExplode, 2);
            break;
        }
        //genera explosion
        case 2: {
            if (expRadio-expGrosor < maxDist) {
                expRadio+=3;
                expGrosor+=.015;
                trans-=.1;
                light_posDOT[2]+=3;
                
                glutPostRedisplay();
                glutTimerFunc(1, arcExplode, 2);
            }
            else {
                expRadio = 0;
            }
            break;
        }
        default:
            break;
    }
    
}

void moverBalas(){
    if (bullet > 0)
        for (int index = 0; index < sizeof(arregloBalas)/sizeof(*arregloBalas); index++) {
            if (arregloBalas[index].viva) {
                float focus_emission [] = {1.0,0.0,0.1,1.0};
                float mat_emissionBala [] = {1.0,1.0,1.0,1.0};
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
                glMaterialf(GL_FRONT, GL_SHININESS, 127.0);
                glColor4fv(mat_emissionBala);
                glDisable(GL_COLOR_MATERIAL);
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
    glOrtho(-screenWidth/2, screenWidth/2, -screenHeight/2, screenHeight/2, -1500, 1500); //izq, der, abajo, arriba, cerca, lejos
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    
}

void myMouse(int button, int state, int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
    //click en el menu, inicia juego con 3 vidas
        if (button == GLUT_LEFT_BUTTON && state == GLUT_UP ){
            switch (menu) {
                case 1:
                    if (x > -119+screenWidth/2 && x < 119+screenWidth/2 && y > -66+screenHeight/2 && y < 66+screenHeight/2) {
                        menu = 0;
                        vidas = 3;
                    }
                    if (x > -184+screenWidth/2 && x < 184+screenWidth/2 && y > -126+screenHeight/2 && y < 126+screenHeight/2) {
                        cout <<"entra";
                    }
                    break;
                case 2:
                    menu = 1;
                default:
                    break;
            }
            
        }
}

void myKeyboard(unsigned char key, int mouseX, int mouseY)
{
    x = mouseX;
    y = screenHeight - mouseY;
    if (!menu) {
        switch (key)
        {
            case ' ':
                if ( explode == 0 ) {
                    glutTimerFunc(5, arcExplode, 1);
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
}

void myPasMouse(int mouseX, int mouseY)
{
    if (menu <= 1) {
        x = mouseX;
        y = screenHeight - mouseY;
        glColor3f( 1.0f, 1.0f, 1.0f );
        xd=x-screenWidth/2;
        yd=y-screenHeight/2;
    }
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
    if (expRadio > 0) {
        float distancia = sqrtf( powf((vectorEnemigos.at(vec)->x-xe), 2)+powf((vectorEnemigos.at(vec)->y-ye), 2));
        if (distancia < expRadio+vectorEnemigos.at(vec)->size/2 && distancia > expRadio-vectorEnemigos.at(vec)->size/2  ){
            vectorEnemigos.at(vec)->vida = 0;
            return 1;
        }
    }
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
                //cambio en x de enemigo
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
                
                //cambio en y de enemigo
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
                    vidas--;
                    if (vidas == 0) {
                        menu = 2;
                    }
                    else {
                        vectorEnemigos.erase(vectorEnemigos.begin()+n);
                        tam--;
                        n--;
                    }
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
    glDisable(GL_LIGHT0);
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,spot_dir);
    glLightfv( GL_LIGHT1, GL_POSITION, light_posDOT );
    glEnable(GL_LIGHT1);
    float colorGrid [] = {1.0,0.0,1.0,1.0};
    //cambia color del grid cuando es golpeado
    if (hit > 0) {
        colorGrid[0] = 1.0;
        colorGrid[1] = 0.0;
        colorGrid[2] = 0.0;
        hit--;
    }
    //color default del grid
    else{
        glColor3f(.1, .1, .1);
        colorGrid[0] = 0.1;
        colorGrid[1] = 0.1;
        colorGrid[2] = 0.1;
    }
    glPushMatrix();
    glTranslatef(0, 0, -1500-screenWidth/2);
    //rotacion inicial
    glRotated(90, 0, 0, 0);
    //rotacion paralax
    glRotated(1, xa, ya, 0);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glEnable(GL_COLOR_MATERIAL);
    glColor4fv(colorGrid);
    glutWireSphere(screenWidth, screenWidth/10, screenHeight/15);
    
    glColor4fv(mat_emission);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glDisable(GL_LIGHT1);
    //glEnable(GL_LIGHT0);
}

void time(int v)
{
    glutPostRedisplay();
    glutTimerFunc(20,time,1);
}

//Makes the image into a texture, and returns the id of the texture
void loadTextureMain(Image* image, int k){
    glBindTexture(GL_TEXTURE_2D, texturasMain[k]);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
}

void pintaLogo(){
    glColor3f(1, 1, 1);
    colorDOT[0] = 1.0;
    colorDOT[1] = 1.0;
    colorDOT[2] = 1.0;
    glPushMatrix();
    glTranslated(0, 100, 0);
    glEnable(GL_COLOR_MATERIAL);
    //D
    glPushMatrix();
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glTranslatef(-180, 0, 0);
    
    glBegin(GL_TRIANGLE_FAN);
    float x1=0,y1=0;
    glVertex2f(x1, y1);
    
    for (float angle = 0; angle< 90; angle+=.1){
        
        if((x1 + sin(angle) * 80)>=0)
            glVertex2f(x1 + sin(angle) * 80, y1 + cos(angle) * 80);
        
    }
    glVertex2f(0, -80);
    glEnd();
    glColor4fv(mat_emission);
    glPopMatrix();
    
    //O
    glPushMatrix();
    glTranslated(0, 0, -80);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glutSolidSphere(80, 50, 50);
    glColor4fv(mat_emission);
    glPopMatrix();
    
    //T
    glPushMatrix();
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glTranslatef(100, 0, 0);
    
    glBegin(GL_QUADS);
    glVertex2f(40,80);
    glVertex2f(40,-80);
    glVertex2f(80,-80);
    glVertex2f(80,80);
    
    glVertex2f(0,20);
    glVertex2f(0,-20);
    glVertex2f(80,-20);
    glVertex2f(80,20);
    glEnd();
    
    glColor4fv(mat_emission);
    glPopMatrix();
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void botonStart(){
    glEnable(GL_COLOR_MATERIAL);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glTranslated(0, -40, 0);
    //hace hexagono negro
    glPushMatrix();
    glBegin(GL_POLYGON);
    glVertex2f( -115, 0 );
    glVertex2f( -92, -24 );
    glVertex2f( 92, -24 );
    glVertex2d( 115, 0);
    glVertex2f( 92, 24 );
    glVertex2d( -92, 24);
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    //hace el bind de la textura
    glPushMatrix();
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glDepthMask(false);
    
    //se carga imagen
    Image* image = loadBMP("Images/start.bmp");
    loadTextureMain(image, 0);
    glBindTexture(GL_TEXTURE_2D, texturasMain[0]);
    //se dibuja imagen con textura
    glBegin(GL_POLYGON);
    glTexCoord2f( 0, 0 );
    glVertex2d( -92, -20 );
    glTexCoord2f( 1, 0 );
    glVertex2d( 92, -20 );
    glTexCoord2f( 1, 1 );
    glVertex2d( 92, 20 );
    glTexCoord2f( 0, 1);
    glVertex2d( -92, 20 );
    glEnd();
    glColor4fv(mat_emission);
    
    glDepthMask( true );
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    //hace el hexagono blanco
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glBegin(GL_POLYGON);
    glVertex2f( -119, 0 );
    glVertex2f( -94, -26 );
    glVertex2f( 94, -26 );
    glVertex2d( 119, 0);
    glVertex2f( 94, 26 );
    glVertex2d( -94, 26);
    glEnd();
    
    glPopMatrix();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_COLOR_MATERIAL);
}

void botonHowto(){
    glEnable(GL_COLOR_MATERIAL);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glTranslated(0, -100, 0);
    //hace hexagono negro
    glPushMatrix();
    float colorHex [] = {0,0,0,0};
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorHex);
    glBegin(GL_POLYGON);
    glVertex2f( -180, 0 );
    glVertex2f( -156, -24 );
    glVertex2f( 156, -24 );
    glVertex2d( 180, 0);
    glVertex2f( 156, 24 );
    glVertex2d( -156, 24);
    glEnd();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    //hace el bind de la textura
    glPushMatrix();
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glEnable(GL_TEXTURE_2D);
    glLoadIdentity();
    glDepthMask(false);
    
    //se carga imagen
    Image* image = loadBMP("Images/howto.bmp");
    loadTextureMain(image, 0);
    glBindTexture(GL_TEXTURE_2D, texturasMain[0]);
    //se dibuja imagen con textura
    glBegin(GL_POLYGON);
    glTexCoord2f( 0, 0 );
    glVertex2d( -156, -20 );
    glTexCoord2f( 1, 0 );
    glVertex2d( 156, -20 );
    glTexCoord2f( 1, 1 );
    glVertex2d( 156, 20 );
    glTexCoord2f( 0, 1);
    glVertex2d( -156, 20 );
    glEnd();
    glColor4fv(mat_emission);
    
    glDepthMask( true );
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    //hace el hexagono blanco
    glPushMatrix();
    glDisable(GL_TEXTURE_2D);
    glColorMaterial(GL_FRONT,GL_EMISSION);
    glColor4fv(colorDOT);
    glBegin(GL_POLYGON);
    glVertex2f( -184, 0 );
    glVertex2f( -158, -26 );
    glVertex2f( 158, -26 );
    glVertex2d( 184, 0);
    glVertex2f( 158, 26 );
    glVertex2d( -158, 26);
    glEnd();
    
    glPopMatrix();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_COLOR_MATERIAL);
}

void pintaMenu(){
    pintaLogo();
    botonStart();
    botonHowto();
}

void display()
{
    //quita el cursor
    glutSetCursor(GLUT_CURSOR_NONE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //genera el grid de background
    crearGrid();
    if (menu == 1) {
        light_posDOT[2] = -500;
        glDisable(GL_LIGHT0);
        pintaMenu();
        glEnable(GL_LIGHT0);
        dibujaDot();
        light_posDOT[0] = 0;
        light_posDOT[1] = 0;
    }
    else if (menu == 2) {
        vectorEnemigos.clear();
        for (int index = 0; index < sizeof(arregloBalas)/sizeof(*arregloBalas); index++) {
            arregloBalas[index].viva = 0;
        }
        xa = 0;
        ya = 0;
        hit = 0;
        carga = M_PI*2;
        light_posDOT[0] = 0;
        light_posDOT[1] = 0;
        colorDOT[0] = 1;
        colorDOT[1] = 1;
        colorDOT[2] = 1;
        //dibuja pantalla de perdiste
        stringstream puntaje;
        puntaje << puntos;
        
        glEnable(GL_COLOR_MATERIAL);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glTranslated(0, 100, 0);
        //hace hexagono negro
        glPushMatrix();
        float colorHex [] = {0,0,0,0};
        glColorMaterial(GL_FRONT,GL_EMISSION);
        glColor4fv(colorHex);
        glBegin(GL_POLYGON);
        glVertex2f( -240, 0 );
        glVertex2f( -175, -94 );
        glVertex2f( 175, -94 );
        glVertex2d( 240, 0);
        glVertex2f( 175, 94 );
        glVertex2d( -175, 94);
        glEnd();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        //hace el bind de la textura
        glPushMatrix();
        glColorMaterial(GL_FRONT,GL_EMISSION);
        glColor4fv(colorDOT);
        glEnable(GL_TEXTURE_2D);
        glLoadIdentity();
        glDepthMask(false);
        
        //se carga imagen
        Image* image = loadBMP("Images/gameover.bmp");
        loadTextureMain(image, 0);
        glBindTexture(GL_TEXTURE_2D, texturasMain[0]);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //se dibuja imagen con textura
        glBegin(GL_POLYGON);
        glTexCoord2f( 0, 0 );
        glVertex2f( -173, -90 );
        glTexCoord2f( 1, 0 );
        glVertex2f( 173, -90 );
        glTexCoord2f( 1, 1 );
        glVertex2f( 173, 90 );
        glTexCoord2f( 0, 1);
        glVertex2f( -173, 90 );
        glEnd();
        glColor4fv(mat_emission);
        
        glDepthMask( true );
        glDisable(GL_TEXTURE_2D);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        //hace el hexagono blanco
        glPushMatrix();
        glDisable(GL_TEXTURE_2D);
        glColorMaterial(GL_FRONT,GL_EMISSION);
        glColor4fv(colorDOT);
        glBegin(GL_POLYGON);
        glVertex2f( -243, 0 );
        glVertex2f( -177, -97 );
        glVertex2f( 177, -97 );
        glVertex2d( 243, 0);
        glVertex2f( 177, 97 );
        glVertex2d( -177, 97);
        glEnd();
        
        glPopMatrix();
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glDisable(GL_COLOR_MATERIAL);
        
        escribirTexto("Your Score: " + puntaje.str(), -55, -25, GLUT_BITMAP_TIMES_ROMAN_24);
    }
    //dibuja Juego
    else if (vidas > 0) {
        pintarVidas();
        pintarMarcador();
        
        glLineWidth(2);
        dibujaDot();
        if (bullet > 0) {
            moverBalas();
        }
        dibujarEnemigos();
    }
    glutSwapBuffers();
}

void init(){
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0,0.0,0.0,0.0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    glEnable(GL_LIGHTING);
    
    //light0
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse_specular);
    glLightfv(GL_LIGHT0,GL_SPECULAR,light_diffuse_specular);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,spot_cutoff);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,spot_exponent);
    glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,1.0);
    glLightfv( GL_LIGHT0, GL_POSITION, light_pos );
    glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spot_dir);
    
    //light1
    glLightfv(GL_LIGHT1,GL_AMBIENT,light_ambient1);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,light_diffuse_specular1);
    glLightfv(GL_LIGHT1,GL_SPECULAR,light_diffuse_specular1);
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,spot_cutoff1);
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,spot_exponent1);
    
    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, mat_ambient_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
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
    glutMouseFunc(myMouse);
    glutKeyboardFunc(myKeyboard);
    glutPassiveMotionFunc(myPasMouse);
	glutMainLoop();
	return 0;
}