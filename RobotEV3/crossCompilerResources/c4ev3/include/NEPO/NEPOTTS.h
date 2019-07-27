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

std::string mapSpeedToEspeakParameter(int speed);
std::string mapPitchToEspeakParameter(int pitch);

inline int generateAudioFile (std::string text, int speed, int pitch)  {
    std::string voice = language + "+f1";
    int childPid = fork();
    if (childPid == 0) {
        char * environment[] = { (char *) LD_LIBRARY_PATH, NULL };
        execle(
            ESPEAK_BIN,
            ESPEAK_BIN,
            "-v", voice.c_str(),
            "-a", "200",
            "-s", mapSpeedToEspeakParameter(speed).c_str(),
            "-p", mapPitchToEspeakParameter(pitch).c_str(),
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

std::string mapSpeedToEspeakParameter(int speed) {
    speed = std::max(0, std::min(speed, 100));
    speed = (((double)speed) * 2.5) + 100;
    return std::to_string(speed);
}

std::string mapPitchToEspeakParameter(int pitch) {
    pitch = std::max(0, std::min(pitch, 100));
    pitch = ((double)pitch) * 0.99;
    return std::to_string(pitch);
}

#endif
