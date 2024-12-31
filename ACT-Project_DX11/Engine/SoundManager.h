#pragma once
#pragma comment(lib, "FMod/fmod_vc.lib")

class SoundManager
{
    DECLARE_SINGLE(SoundManager);

public:
    bool Initialize();
    void Release();

    bool Load(wstring key, wstring filename, bool isStream = false);
    void Play(wstring key, bool loop = false);
    void PlayFromTime(wstring key, float startTime, bool loop);
    void Stop(wstring key);
    void SetVolume(wstring key, float volume);
    void Update();
    void Paused(wstring key, bool pause);
    void PauseAll(bool pause);
    void PlayEffect(wstring key);

private:
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    FMOD::System* m_pSystem = nullptr; 
    std::vector<FMOD::Channel*> m_ActiveChannels;
    std::map<wstring, FMOD::Sound*> m_SoundMap;
    std::map<wstring, FMOD::Channel*> m_ChannelMap;
};