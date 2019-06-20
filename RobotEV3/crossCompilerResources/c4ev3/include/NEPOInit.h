#ifndef NEPOINIT
#define NEPOINIT

#include <ev3.h>

#define JUST_UPLOADED_FLAG_FILE_NAME "../prjs/BrkProg_SAVE/NEPO-just-uploaded.txt"

bool NEPOProgramJustUploaded();
void onNEPOProgramJustUploaded ();

void inline NEPOInitEV3 () {
    InitEV3();
    if (NEPOProgramJustUploaded()) {
        onNEPOProgramJustUploaded();
    }
}

bool NEPOProgramJustUploaded () {
     return false;
     //return access(JUST_UPLOADED_FLAG_FILE_NAME, F_OK) == 0;
}


void deleteJustUploadedFlag();

void onNEPOProgramJustUploaded () {
    PlaySound(SOUND_DOUBLE_BEEP);
    Wait(1000);
    deleteJustUploadedFlag();
    FreeEV3();
    exit(0);
}

void deleteJustUploadedFlag() {
    remove(JUST_UPLOADED_FLAG_FILE_NAME);
}

#endif