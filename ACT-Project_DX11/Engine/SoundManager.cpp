#include "pch.h"
#include "SoundManager.h"
#include "Utils.h"

bool SoundManager::Load(wstring key, wstring filename, bool isStream)
{
    FMOD::Sound* sound = nullptr;
    FMOD_MODE mode = isStream ? FMOD_CREATESTREAM : FMOD_DEFAULT;

    wstring fullpath = L"../Resources/Sound/" + filename + L".wav";

    FMOD_RESULT result = m_pSystem->createSound(Utils::ToString(fullpath).c_str(), mode, nullptr, &sound);
    if (result != FMOD_OK) {
        std::cout << "Failed to load sound: " << &key << std::endl;
        return false;
    }

    m_SoundMap[key] = sound;
    return true;
}

void SoundManager::Play(wstring key, bool loop)
{
    auto it = m_SoundMap.find(key);
    if (it == m_SoundMap.end()) {
        std::cout << "Sound not found: " << &key << std::endl;
        return;
    }

    FMOD::Sound* sound = it->second;
    if (loop) {
        sound->setMode(FMOD_LOOP_NORMAL);
    }
    else {
        sound->setMode(FMOD_LOOP_OFF);
    }

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_pSystem->playSound(sound, nullptr, false, &channel);
    if (result != FMOD_OK) {
        std::cout << "Failed to play sound: " << &key << std::endl;
        return;
    }

    channel->setPaused(false); // 일시정지 해제
    m_ChannelMap[key] = channel;
}

void SoundManager::PlayFromTime(wstring key, float startTime, bool loop)
{
    auto it = m_SoundMap.find(key);
    if (it == m_SoundMap.end()) {
        std::cerr << "Sound not found: " << &key << std::endl;
        return;
    }

    FMOD::Sound* sound = it->second;

    if (loop) {
        sound->setMode(FMOD_LOOP_NORMAL);
    }
    else {
        sound->setMode(FMOD_LOOP_OFF);
    }

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_pSystem->playSound(sound, nullptr, true, &channel); // 재생을 일시정지 상태로 시작
    if (result != FMOD_OK) {
        std::cerr << "Failed to play sound: " << &key << std::endl;
        return;
    }

    float startTimems = startTime * 1000;
    // 특정 시간부터 재생 시작
    result = channel->setPosition(startTimems, FMOD_TIMEUNIT_MS);
    if (result != FMOD_OK) {
        std::cerr << "Failed to set position for sound: " << &key << std::endl;
        channel->stop();
        return;
    }

    channel->setPaused(false);
    m_ChannelMap[key] = channel;

    std::cout << "Playing sound: " << &key << " from " << startTime << "ms" << std::endl;

}

void SoundManager::PlayEffect(wstring key)
{
    auto it = m_SoundMap.find(key);
    if (it == m_SoundMap.end()) {
        std::cerr << "Effect sound not found: " << &key << std::endl;
        return;
    }

    FMOD::Sound* sound = it->second;

    // 루프 없이 재생
    sound->setMode(FMOD_LOOP_OFF);

    FMOD::Channel* channel = nullptr;
    FMOD_RESULT result = m_pSystem->playSound(sound, nullptr, false, &channel);
    if (result != FMOD_OK) {
        std::cerr << "Failed to play effect sound: " << &key << std::endl;
        return;
    }

    std::cout << "Playing effect sound: " << &key << std::endl;
}

void   SoundManager::SetVolume(wstring key, float volume)
{
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;

    auto it = m_ChannelMap.find(key);
    if (it != m_ChannelMap.end() && it->second) {
        it->second->setVolume(volume);
    }
}

void   SoundManager::Paused(wstring key, bool pause) // 토클기능 on or off
{
    auto it = m_ChannelMap.find(key);
    if (it != m_ChannelMap.end() && it->second) {
        FMOD_RESULT result = it->second->setPaused(pause);
        if (result != FMOD_OK) {
            std::cerr << "Failed to pause sound: " << std::endl;
        }
        else {
            std::cout << (pause ? "Paused" : "Resumed") << " sound: "<< &key << std::endl;
        }
    }
    else {
        std::cerr << "Channel not found or invalid for key: " << &key << std::endl;
    }
}

void   SoundManager::Stop(wstring key)
{
    auto it = m_ChannelMap.find(key);
    if (it != m_ChannelMap.end() && it->second) {
        it->second->stop();
    }
}

bool SoundManager::Initialize()
{
    FMOD_RESULT hr = FMOD::System_Create(&m_pSystem);
    if (hr != FMOD_OK) {
        std::cout << "FMOD system creation failed!" << std::endl;
        return false;
    }

    hr = m_pSystem->init(512, FMOD_INIT_NORMAL, nullptr);
    if (hr != FMOD_OK) {
        std::cout << "FMOD system initialization failed!" << std::endl;
        return false;
    }

    return true;
}

void   SoundManager::Release()
{
    for (auto& soundPair : m_SoundMap) {
        soundPair.second->release();
    }
    m_SoundMap.clear();

    if (m_pSystem) {
        m_pSystem->close();
        m_pSystem->release();
    }
}

void   SoundManager::Update()
{
    if (m_pSystem) {
        m_pSystem->update();
    }
}