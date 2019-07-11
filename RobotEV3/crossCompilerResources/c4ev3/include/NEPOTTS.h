#ifndef NEPOTTS
#define NEPOTTS

#define SYNTHETIZE_SCIRPT_FILE_NAME "/media/card/NEPO-TTS/synthetize.sh"
#define GENERATED_AUDIO_FILE "/media/card/NEPO-TTS/tmp/output.wav"

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
    // TODO: Fix escaping
     std::string cmd = SYNTHETIZE_SCIRPT_FILE_NAME;
     cmd += " ";
     cmd += language;
     cmd += " \"";
     cmd += text;
     cmd += "\" ";
     cmd += ToString(speed);
     cmd += " ";
     cmd += ToString(pitch);

     return system(cmd.c_str());

}


#endif
