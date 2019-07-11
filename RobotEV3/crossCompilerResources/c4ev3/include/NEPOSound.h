static int NEPOSoundVolume = 100;

inline void SetVolume(int volume) {
    NEPOSoundVolume = volume;
}

inline int GetVolume() {
    return NEPOSoundVolume;
}

inline void PlaySystemSound (int soundName) {
    PlaySoundEx(soundName, NEPOSoundVolume);
}

inline void NEPOPlayTone (unsigned short frequency, unsigned short duration) {
    PlayToneEx(frequency, duration, NEPOSoundVolume);
    Wait(duration);
}
