#ifndef NEPOTTS
#define NEPOTTS

#include <sys/types.h>
#include <sys/wait.h>


#define ESPEAK_BIN "/media/card/NEPO-TTS/speak-ng"
#define LD_LIBRARY_PATH "LD_LIBRARY_PATH=\"/media/card/NEPO-TTS\""
#define GENERATED_AUDIO_FILE "/media/card/NEPO-TTS/output.wav"

static std::string language = "en";

void SetLanguage(std::string toSet) {
    language = toSet;
}

int generateAudioFile (std::string text, int speed, int pitch);
void notifyError();

inline void Say (std::string text, int speed, int pitch) {
    int res = generateAudioFile(text, speed, pitch);
    if (res != 0) {
        NEPOError("NEPO-TTS not found!", "Please copy the folder into the SD card");
    } else {
        PlayFileEx((char *) GENERATED_AUDIO_FILE, GetVolume(), false);
    }
}


inline void Say (std::list<std::string> strings, int speed, int pitch) {
    for (auto str : strings) {
        Say(str, speed, pitch);
    }
}


inline int generateAudioFile (std::string text, int speed, int pitch)  {
    int childPid = fork();
    if (childPid == 0) {
        char * environment[] = { LD_LIBRARY_PATH, (char *) 0 };
        execle(
            ESPEAK_BIN,
            ESPEAK_BIN,
            "-v", language.c_str(),
            "-s", ToString(speed).c_str(),
            "-p", ToString(pitch).c_str(),
            "-w", GENERATED_AUDIO_FILE,
            text.c_str(),
            (char *) NULL,
            environment
        );
        exit(-1);
    } else {
        int exitStatus;
        int exitedChild;
        do {
            exitedChild = wait(&exitStatus);
        } while (exitedChild != childPid);
        return WIFEXITED(exitStatus) && WEXITSTATUS(exitStatus);
    }
}

#endif
