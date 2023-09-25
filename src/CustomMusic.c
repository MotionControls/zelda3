#include "CustomMusic.h"

/*void MUS_LoadMusic(char* path){
    gMusic_Playing = Mix_LoadMUS(path);
    Mix_PlayMusic(gMusic_Playing, 0);
    printf("%s.\n", Mix_GetError());
}*/

/*  MUS_LoadBasedContext();
    Loads a music file based on certain gamestates.
    Requires all songs to be defined first.
    Returns 0 if no suitable song is found.
*/
int MUS_LoadBasedContext(){

    Mix_PauseMusic();
    char* load_music_key = malloc(sizeof(char)*512);
    load_music_key[0] = NULL;
    char *extra_id;

    extra_id = malloc(sizeof(char)*2);
    sprintf(extra_id, "%d", music_control);
    //printf("extra_id: %s.\n", extra_id);
    strcat(load_music_key, "music.game.\0");
    strcat(load_music_key, extra_id);

    return MUS_Load(load_music_key, 0);
}

/*  MUS_Load(filePath, doInterrupt);
    Loads a music file based on given params.
    Returns 0 if file isn't found.
filePath    ;   Path to music file.
doInerrupt  ;   Whether or not the loaded track should interrupt the current track.
*/
int MUS_Load(const char* key, int doInterrupt){
    if(!gLoadedSoundModFile) return 0;

    json_value* music_info = GetValueByKey(key, gLoadedSoundModFile);

    if(!music_info) {
        printf("Failed to locate key for track \'%s\'.\n", key);
        return 0;
    }

    char* desired_path = malloc(sizeof(char)*_MAX_PATH);
    double *desired_start = malloc(sizeof(double)), *desired_end = malloc(sizeof(double)), *desired_loop = malloc(sizeof(double));
    int *should_do_loop = malloc(sizeof(int)), *should_do_interrupt = malloc(sizeof(int));

    *desired_end = -1;
    *desired_loop = -1;

    json_value* desired_path_value = GetValueByKey("path", music_info);
    json_value* desired_start_value = GetValueByKey("start_at", music_info);
    json_value* desired_end_value = GetValueByKey("end_at", music_info);
    json_value* desired_loop_value = GetValueByKey("loop_point", music_info);
    json_value* should_do_loop_value = GetValueByKey("do_loop", music_info);
    json_value* should_do_interrupt_value = GetValueByKey("do_interrupt", music_info);

    if(!desired_path_value) {
        printf("Sound key \'%s\' does not have a set music path, cannot load.\n", key);
        return 0;
    }

    strcpy(desired_path, desired_path_value->u.string.ptr);

    if(desired_start_value) {
        printf("doingInterrupt: %i.\n", doingInterrupt);
        if(doingInterrupt)
            *desired_start = curMusicPos_Back;
        else
            *desired_start = desired_start_value->u.integer;
    }

    if(desired_end_value) {
        *desired_end = desired_end_value->u.integer;
    }

    if(desired_loop_value) {
        *desired_loop = desired_loop_value->u.integer;
    }

    if(should_do_loop_value) {
        *should_do_loop = should_do_loop_value->u.boolean;
    }

    if(should_do_interrupt_value) {
        *should_do_interrupt = should_do_interrupt_value->u.boolean;
        printf("should_do_interrupt: %i.\n", *should_do_interrupt);
        if(*should_do_interrupt){
            curMusicPos_Back = curMusicPos;
            isInterrupt = 1;
            printf("curMusicPos_Back: %f.\n", curMusicPos_Back);
        }
    }

    int ret_value = MUS_LoadFile(desired_path, desired_start, desired_end, desired_loop, should_do_loop, should_do_interrupt);
    free(desired_start);
    free(desired_path);
    free(desired_end);
    free(desired_loop);
    free(should_do_interrupt);
    free(should_do_loop);

    return ret_value;
}

/*  MUS_LoadFile(path, start, end, loop, doLoop);
    Loads a music file based on the given params.
path            ;   Path to MP3.
start,end,loop  ;   Timing points.
doLoop          ;   Whether or not the track should loop.
doInterrupt     ;   Whether or not the track should interrupt the current track.
*/
int MUS_LoadFile(const char* path, double* start, double* end, double* loop, int* doLoop, int* doInterrupt){
    // Load MP3.
    gMusic_Playing = Mix_LoadMUS(path);
    if(gMusic_Playing == NULL){
        printf("MUS_Load() Err: Mix_LoadMUS() Err: %s.\n", Mix_GetError());
        return 0;
    }

    // Store interrupt data if applicable.
    if(*doInterrupt && USE_MUSICVAR_BACK){
        isInterrupt = 1;
        curTrackPath_Back = curTrackPath;
        curMusicPos_Back = curMusicPos;
        curMusicLoopPoint_Back = curMusicLoopPoint;
        curMusicEndPoint_Back = curMusicEndPoint;
        curMusicDoLoop_Back = curMusicDoLoop;
    }else{
        isInterrupt = 0;
    }
    curTrackPath = path;

    bool defaultLoop = (loop == NULL), defaultEnd = (end == NULL);

    if(!defaultLoop) defaultLoop = (*loop < 0);
    if(!defaultEnd) defaultEnd = (*end < 0);

    // Tracking vars.
    curMusicPos = *start;
    if(defaultLoop)
        curMusicLoopPoint = Mix_MusicDuration(gMusic_Playing) * 1000;
    else
        curMusicLoopPoint = *loop;
    if(defaultEnd)
        curMusicEndPoint = Mix_MusicDuration(gMusic_Playing) * 1000;
    else
        curMusicEndPoint = *end;
    curMusicDoLoop = *doLoop;

    printf("MUS_LoadFile(): Loading music with vars: %f, %f, %f, %i, %i.\n", curMusicPos, curMusicEndPoint, curMusicLoopPoint, curMusicDoLoop, isInterrupt);

    // Play music.
    Mix_PlayMusic(gMusic_Playing, 0);
    Mix_SetMusicPosition(curMusicPos / 1000);
    Mix_ResumeMusic();

    return 1;
}

/*  MUS_Step();
    Tracks current music.
*/
void MUS_Step(){
    if(gMusic_Playing != NULL && !Mix_PausedMusic()){
        //double temp = Mix_GetMusicPosition(gMusic_Playing) * 1000;
        curMusicPos = Mix_GetMusicPosition(gMusic_Playing) * 1000;//temp;
    }
    
    //printf("Stuff: %f / %f / %f.\n", curMusicEndPoint, curMusicLoopPoint, curMusicPos);
    //printf("counter_global_frames: %u.\n", counter_global_frames);


    if(curMusicPos >= curMusicEndPoint && curMusicDoLoop)
        Mix_SetMusicPosition((double)curMusicLoopPoint / 1000);
}