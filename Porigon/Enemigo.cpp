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
        switch (tipo) {
            case 1:
                glBegin(GL_POLYGON);
                glVertex2d(x-size/2, y-size/2);
                glVertex2d(x+size/2, y-size/2);
                glVertex2d(x, y+size/2);
                glEnd();
                break;
            case 2:
            {
                Image* image = loadBMP("/Users/Dave/Dropbox/Tareas Cloud/7º Semestre/Graficas/Porigon/Porigon/Textura1.bmp");
                loadTexture(image, 0);
                
                glBindTexture(GL_TEXTURE_2D, texturas[0]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                glBegin(GL_QUADS);
                glNormal3f( 0.0f, 0.0f, 1.0f);
                glTexCoord2f(0.0f, 0.0f);
                glVertex3f(x-size/2, y-size/2,  1.0f);
                glTexCoord2f(2.0f, 0.0f);
                glVertex3f(x+size/2, y-size/2,  1.0f);
                glTexCoord2f(2.0f, 2.0f);
                glVertex3f(x+size/2, y+size/2,  1.0f);
                glTexCoord2f(0.0f, 2.0f);
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

#include <assert.h>
#include <fstream>

#include "imageloader.h"

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h)
{
    
}

Image::~Image()
{
    delete[] pixels;
}

namespace
{
    //Converts a four-character array to an integer, using little-endian form
    int toInt(const char* bytes)
    {
        return (int)(((unsigned char)bytes[3] << 24) |
                     ((unsigned char)bytes[2] << 16) |
                     ((unsigned char)bytes[1] << 8) |
                     (unsigned char)bytes[0]);
    }
    
    //Converts a two-character array to a short, using little-endian form
    short toShort(const char* bytes)
    {
        return (short)(((unsigned char)bytes[1] << 8) |
                       (unsigned char)bytes[0]);
    }
    
    //Reads the next four bytes as an integer, using little-endian form
    int readInt(ifstream &input)
    {
        char buffer[4];
        input.read(buffer, 4);
        return toInt(buffer);
    }
    
    //Reads the next two bytes as a short, using little-endian form
    short readShort(ifstream &input)
    {
        char buffer[2];
        input.read(buffer, 2);
        return toShort(buffer);
    }
    
    //Just like auto_ptr, but for arrays
    template<class T>
    class auto_array
    {
    private:
        T* array;
        mutable bool isReleased;
    public:
        explicit auto_array(T* array_ = NULL) :
        array(array_), isReleased(false)
        {
        }
        
        auto_array(const auto_array<T> &aarray)
        {
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }
        
        ~auto_array()
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
        }
        
        T* get() const
        {
            return array;
        }
        
        T &operator*() const
        {
            return *array;
        }
        
        void operator=(const auto_array<T> &aarray)
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }
        
        T* operator->() const
        {
            return array;
        }
        
        T* release()
        {
            isReleased = true;
            return array;
        }
        
        void reset(T* array_ = NULL)
        {
            if (!isReleased && array != NULL)
            {
                delete[] array;
            }
            array = array_;
        }
        
        T* operator+(int i)
        {
            return array + i;
        }
        
        T &operator[](int i)
        {
            return array[i];
        }
    };
}

Image* loadBMP(const char* filename)
{
    ifstream input;
    input.open(filename, ifstream::binary);
    assert(!input.fail() || !"Could not find file");
    char buffer[2];
    input.read(buffer, 2);
    assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
    input.ignore(8);
    int dataOffset = readInt(input);
    
    //Read the header
    int headerSize = readInt(input);
    int width;
    int height;
    switch (headerSize)
    {
        case 40:
            //V3
            width = readInt(input);
            height = readInt(input);
            input.ignore(2);
            assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
            assert(readShort(input) == 0 || !"Image is compressed");
            break;
        case 12:
            //OS/2 V1
            width = readShort(input);
            height = readShort(input);
            input.ignore(2);
            assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
            break;
        case 64:
            //OS/2 V2
            assert(!"Can't load OS/2 V2 bitmaps");
            break;
        case 108:
            //Windows V4
            assert(!"Can't load Windows V4 bitmaps");
            break;
        case 124:
            //Windows V5
            assert(!"Can't load Windows V5 bitmaps");
            break;
        default:
            assert(!"Unknown bitmap format");
    }
    
    //Read the data
    int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
    int size = bytesPerRow * height;
    auto_array<char> pixels(new char[size]);
    input.seekg(dataOffset, ios_base::beg);
    input.read(pixels.get(), size);
    
    //Get the data into the right format
    auto_array<char> pixels2(new char[width * height * 3]);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                pixels2[3 * (width * y + x) + c] =
                pixels[bytesPerRow * y + 3 * x + (2 - c)];
            }
        }
    }
    
    input.close();
    return new Image(pixels2.release(), width, height);
}
