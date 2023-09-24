#ifndef CUSTOMMUSIC_H
#define CUSTOMMUSIC_H

#include "types.h"
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include "variables.h"

/* Music Vars */
Mix_Music* gMusic_Playing;

/* Music Funcs */
void MUS_LoadMusic(char*);

#endif