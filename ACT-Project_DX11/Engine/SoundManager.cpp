#include "pch.h"
#include "SoundManager.h"
#include "Utils.h"

bool   SoundManager::Load(std::wstring filename)
{
    m_fVolume = 0.5f;  // 0 ~ 1	
    FMOD_RESULT hr = m_pFmodSystem->createSound(Utils::ToString(filename).c_str(),
        FMOD_DEFAULT, 0, &m_pSound);
    if (hr == FMOD_OK)
    {
        return true;
    }
    return false;
}

void  SoundManager::PlayEffect() // 단순한(짧은) 이펙트 용도
{
    FMOD::Channel* pChannel = nullptr;
    FMOD_RESULT hr = m_pFmodSystem->playSound(m_pSound,
        nullptr, false, &pChannel);
    if (hr == FMOD_OK)
    {
    }
}
FMOD::Channel* SoundManager::Play(bool bLoop) // 트루일 시 무한 반복
{
    // 채널 : 실행되는 사운드 제어를 담당한다.
    FMOD::Channel* pChannel = nullptr;
    FMOD_RESULT hr = m_pFmodSystem->playSound(m_pSound,
        nullptr, false, &pChannel);

    m_pChannel = pChannel;
    if (hr == FMOD_OK)
    {
        m_pChannel->setVolume(m_fVolume);
        m_pSound->getLength(&m_SizeMS, FMOD_TIMEUNIT_MS);
        if (bLoop)
        {
            m_pChannel->setMode(FMOD_LOOP_NORMAL); // 무한반복
        }
        else
        {
            m_pChannel->setMode(FMOD_LOOP_OFF);
        }
    }
    return pChannel;
}

void   SoundManager::Stop()
{
    if (m_pChannel != nullptr)
    {
        m_pChannel->stop();
    }
}