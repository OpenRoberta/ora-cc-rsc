#ifndef NEPOINIT
#define NEPOINIT

#include <ev3.h>

#define POOL_ESCAPE_SEQUENCE 100

#define JUST_UPLOADED_FLAG_FILE_NAME "../prjs/BrkProg_SAVE/NEPO-just-uploaded.txt"

bool NEPOProgramJustUploaded();
void onNEPOProgramJustUploaded ();
void startThreadToMonitorEscapeSequence();

void inline NEPOInitEV3 () {
    InitEV3();
    if (NEPOProgramJustUploaded()) {
        onNEPOProgramJustUploaded();
    }
    startThreadToMonitorEscapeSequence();
}

void inline NEPOFreeEV3 () {
    FreeEV3();
    exit(0);
}

bool NEPOProgramJustUploaded () {
     return access(JUST_UPLOADED_FLAG_FILE_NAME, F_OK) == 0;
}

void deleteJustUploadedFlag();

void onNEPOProgramJustUploaded () {
    PlaySound(SOUND_DOUBLE_BEEP);
    Wait(1000);
    deleteJustUploadedFlag();
    NEPOFreeEV3();
}

void deleteJustUploadedFlag() {
    remove(JUST_UPLOADED_FLAG_FILE_NAME);
}


void * escapeSequenceMonitor (void * arguments);

void startThreadToMonitorEscapeSequence () {
     pthread_t threadId;
     pthread_create(&threadId, NULL, escapeSequenceMonitor, NULL);
}

bool isEscapeSequence ();

void * escapeSequenceMonitor (void * arguments) {
    while (!isEscapeSequence()) {
        Wait(POOL_ESCAPE_SEQUENCE);
    }
    NEPOFreeEV3();
}

bool isEscapeSequence () {
    return ButtonIsDown(BTNCENTER) && ButtonIsDown(BTNDOWN);
}

#endif