#include "CustomMusic.h"

void MUS_LoadMusic(char* path){
    gMusic_Playing = Mix_LoadMUS(path);
    Mix_PlayMusic(gMusic_Playing, 0);
    printf("%s.\n", Mix_GetError());
}