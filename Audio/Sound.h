//
//  Sound.h
//  Porigon
//
//  Created by Dave on 11/19/13.
//  Copyright (c) 2013 Dave. All rights reserved.
//

#ifndef __Porigon__Sound__
#define __Porigon__Sound__

#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

using namespace std;

class Sound {
public:
    void *data;
    unsigned int bufferID;
    unsigned int sourceID;
    int format;
    int sampleRate;
    int size;
    
    Sound(int format, int sampleRate, void* data, int size);
    void play();
    void stop();
    static Sound* loadWave(const char* filename);
};
#endif /* defined(__Porigon__Sound__) */
