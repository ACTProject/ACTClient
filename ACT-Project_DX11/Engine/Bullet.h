#pragma once
class Model;
class Transform;
class GameObject;

class Bullet: public Component
{
    using Super = Component;

public:
    Bullet();
    virtual ~Bullet();

    void Update();
    shared_ptr<Model> GetObjects () { return _bullet; }
    void SetHitBox(shared_ptr<GameObject> hitbox) { _hitbox = hitbox; }
    void Add(shared_ptr<Model> bullet) { _bullet = bullet; }
    void SetSpeed(float _speed) { speed = _speed; }
    void SetDirection(Vec3 Dir) { direction = Dir; }

    void Shooting();

    Vec3 direction;   // 총알 이동 방향
    Vec3 CurForward;
    float speed;      // 총알 속도
    float duration = 0.0f;
    Vec3 PlayerPos;
    bool FirstTime = true;
    bool FirstTime_2 = true;
    shared_ptr<Model> _bullet;

private:
    shared_ptr<GameObject> _hitbox;
    shared_ptr<Transform> _transform;
    shared_ptr<GameObject> _player;
};

