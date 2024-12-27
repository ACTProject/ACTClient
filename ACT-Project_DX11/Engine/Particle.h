#pragma once
#include "Component.h"

struct ParticleType
{
    Vec3 position;
    Vec3 velocity;
    float lifetime;
    float age;
};

struct VertexParticle
{
    Vec3 position;
    Vec2 uv;
    Vec2 scale;
};

#define MAX_BILLBOARD_COUNT 500


class Particle : public Component
{
	using Super = Component;

public:
    Particle();
	~Particle();

public:
    void Update();
    void Add(Vec3 position, Vec2 scale);

    void SetLifetime(float lifetime) { _lifetime = lifetime; }
    void SetfadeStart(float fadeStart) { _fadeStart = fadeStart; }
    void SetReuse(float isReusable) { _isReusable = isReusable; }

    void SetMaterial(shared_ptr<Material> material) { _material = material; }
    void SetPass(uint8 pass) { _pass = pass; }
    shared_ptr<Material> GetMaterial() { return _material; }

private:
    vector<VertexParticle> _vertices;
    vector<uint32> _indices;
    shared_ptr<VertexBuffer> _vertexBuffer;
    shared_ptr<IndexBuffer> _indexBuffer;

    int32 _drawCount = 0;
    int32 _prevCount = 0;

    uint8 _pass = 0;
    float _elapsedTime = 0.f;

    shared_ptr<Material> _material;

    float _lifetime = 3.0f;
    float _fadeStart = 1.0f;

    bool _isReusable = false;
};

