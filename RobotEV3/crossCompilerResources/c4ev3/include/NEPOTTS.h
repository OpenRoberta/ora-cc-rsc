#ifndef NEPOTTS
#define NEPOTTS

#define SYNTHETIZE_SCIRPT_FILE_NAME "/media/card/NEPO-TTS/synthetize.sh"
#define GENERATED_AUDIO_FILE "/media/card/NEPO-TTS/tmp/output.wav"

int generateAudioFile (std::string text, std::string language, int speed, int pitch);

inline void Say (std::string text, std::string language, int speed, int pitch) {
    generateAudioFile(text, language, speed, pitch);
    PlayFileEx((char *) GENERATED_AUDIO_FILE, GetVolume(), false);
}


inline void Say (std::list<std::string> strings, std::string language, int speed, int pitch) {
    for (auto str : strings) {
        Say(str, language, speed, pitch);
    }
}


inline int  generateAudioFile (std::string text, std::string language, int speed, int pitch)  {
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
