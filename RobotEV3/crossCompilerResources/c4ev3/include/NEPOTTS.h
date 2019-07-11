#ifndef NEPOTTS
#define NEPOTTS

#define ESPEAK_PATH "/media/card/NEPO-TTS"
#define GENERATED_AUDIO_FILE "/media/card/NEPO-TTS/output.wav"

static std::string language = "en";

void SetLanguage(std::string toSet) {
    language = toSet;
}

int generateAudioFile (std::string text, int speed, int pitch);

inline void Say (std::string text, int speed, int pitch) {
    generateAudioFile(text, speed, pitch);
    PlayFileEx((char *) GENERATED_AUDIO_FILE, GetVolume(), false);
}


inline void Say (std::list<std::string> strings, int speed, int pitch) {
    for (auto str : strings) {
        Say(str, speed, pitch);
    }
}


inline int  generateAudioFile (std::string text, int speed, int pitch)  {
     std::string cmd = "LD_LIBRARY_PATH=\"" + (std::string) ESPEAK_PATH + "\" ";
     cmd += ESPEAK_PATH + (std::string) "/speak-ng";
     cmd += " -v " + language;
     cmd += " -s " + (std::string) ToString(speed);
     cmd += " -p " + (std::string) ToString(pitch);
     cmd += " -w " + (std::string) GENERATED_AUDIO_FILE;
     cmd += " \"";
     cmd += text;
     cmd += "\""; // TODO: Improve escaping

     return system(cmd.c_str());
}


#endif
