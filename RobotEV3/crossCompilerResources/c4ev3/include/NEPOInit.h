#ifndef NEPOINIT
#define NEPOINIT

#include <ev3.h>

#define POOL_ESCAPE_SEQUENCE 100

#define JUST_UPLOADED_FLAG_FILE_NAME "../prjs/BrkProg_SAVE/NEPO-just-uploaded.txt"

bool NEPOProgramJustUploaded();
void onNEPOProgramJustUploaded ();
void startThreadToMonitorEscapeSequence();

bool initialized = false;

void inline NEPOInitEV3 () {
    if (initialized) {
        return;
    }
    initialized = true;
    InitEV3();
    if (NEPOProgramJustUploaded()) {
        onNEPOProgramJustUploaded();
    }
    startThreadToMonitorEscapeSequence();
    SetLedPattern(LED_GREEN_PULSE);
}

void inline NEPOFreeEV3 () {
    SetLedPattern(LED_GREEN);
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
    initialized = false;
    return NULL;
}

bool isEscapeSequence () {
    return ButtonIsDown(BTNCENTER) && ButtonIsDown(BTNDOWN);
}

void NEPOError (std::string error, std::string message) {
    SetLedPattern(LED_RED);
    LcdClean();
    DrawString("ERROR", 8, 2);
    DrawString(error.c_str(), 10 - (error.length() / 2), 4);
    DrawString(message.c_str(), 0, 8);
    DrawString("CENTER + DOWN to exit", 1, 11);
    PlaySystemSound(SOUND_LOW_BEEP);
    while(true) {
        Wait(1000);
    }
}


#endif