#pragma once
#include "Component.h"

struct ParticleType
{
    Vec3 position;
    Vec3 velocity;
    float lifetime;
    float age;
};

class Particle : public Component
{
	using Super = Component;

public:
    Particle();
	~Particle();

public:

    void Create(Vec3 screenPos, Vec2 size, shared_ptr<Material> material);
    void Update();

private:
    float _elapsedTime = 0.f;

    shared_ptr<Material> _material;

    float _lifetime = 10.0f;
    float _fadeStart = 2.0f;

    
};

