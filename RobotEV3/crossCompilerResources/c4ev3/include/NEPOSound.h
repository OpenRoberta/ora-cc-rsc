static int _soundVolume = 100;

inline void SetVolume(int volume) {
    _soundVolume = volume;
}

inline void _PlaySound (int soundName) {
    PlaySoundEx(soundName, _soundVolume);
}
