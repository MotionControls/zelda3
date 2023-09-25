#ifndef CUSTOMMUSIC_H
#define CUSTOMMUSIC_H

#include "types.h"
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include "variables.h"
#include "third_party/libs/json.h"

/* FileData Vars */
json_value* gLoadedSoundModFile;

/* Music Vars */
const char* curTrackPath;                   // Path to whatever is currently playing.
double curMusicPos;                         // Music position in ms.
double curMusicLoopPoint, curMusicEndPoint; // Where music should loop and end in ms. 
int curMusicDoLoop;                         // Whether or not the current song should loop.
int isInterrupt;                            // Whether or not the current track interrupted a previous track.
int doingInterrupt;
Mix_Music* gMusic_Playing;                  // The currently playing track.

// The vars below are used to keep track of where the last playing track left off.
// Non-vanilla behavior. These are only used if allowed.
const char* curTrackPath_Back;
double curMusicPos_Back;
double curMusicLoopPoint_Back, curMusicEndPoint_Back;
int curMusicDoLoop_Back;

/* Music Funcs */
//void MUS_LoadMusic(char*);
int MUS_LoadBasedContext();
int MUS_Load(const char*,int);
int MUS_LoadFile(const char*,double*,double*,double*,int*,int*);
void MUS_Step();

/* Sound Funcs */
/*
void SND_Load();
int SND_Step_Ch1();
int SND_Step_Ch2();
int SND_Step_Ch3();
*/

#endif