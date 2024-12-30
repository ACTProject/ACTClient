#pragma once
#pragma comment(lib, "FMod/fmod_vc.lib")

class SoundManager
{
    DECLARE_SINGLE(SoundManager);

private:
    FMOD::System* m_pFmodSystem = nullptr;
public:
    void SetFmod(FMOD::System* pSystem)
    {
        m_pFmodSystem = pSystem;
    }
    FMOD::Sound*    m_pSound = nullptr;
    FMOD::Channel*  m_pChannel = nullptr;
    float		    m_fVolume = 1.0f;
    unsigned int	m_SizeMS;
public:
    bool   Load(std::wstring filename);
    FMOD::Channel* Play(bool bLoop = false);
    void   PlayEffect();
    void   SetVolume(float value);
    void   Stop();
    void   Paused();
    void   Release();
};