#pragma once
#include "Camera.h"

struct CutsceneEvent {
    Vec3 start, end, focus;
    float duration;
};

class CutsceneManager {
    DECLARE_SINGLE(CutsceneManager);

public:
    void AddEvent(CutsceneEvent event) {
        _eventQueue.push(event);
    }

    void PlayNext() {
        if (IsQueueEmpty()) {
            std::cout << "All cutscenes played." << std::endl;
            return;
        }

        auto event = _eventQueue.front();
        _eventQueue.pop();
        _camera->StartCutscene(event.start, event.end, event.focus, event.duration);
    }

    bool IsQueueEmpty() const {
        return _eventQueue.empty();
    }

    void SetCamera(const shared_ptr<Camera>& camera) {
        _camera = camera;
    }

private:
    queue<CutsceneEvent> _eventQueue;
    shared_ptr<Camera> _camera;
};

