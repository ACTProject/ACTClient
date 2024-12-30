#pragma once
#pragma comment(lib, "fmod_vc.lib")

class SoundManager
{
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
    wchar_t			m_msSound[MAX_PATH] = { 0, };
    wchar_t			m_msPlay[MAX_PATH] = { 0, };
public:
    bool   Load(std::wstring filename);
    FMOD::Channel* Play(bool bLoop = false);
    void   PlayEffect();
    void   Stop();
    void   Paused();
    void   VolumeUp(float fVolume);
    void   VolumeDown(float fVolume);
};

