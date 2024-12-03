﻿#pragma once

class BaseCollider;
class Rigidbody;

class CollisionManager
{
	DECLARE_SINGLE(CollisionManager);

public:
	void Init();
	void Update();

	// Collider & Rigidbody 등록
	void AddCollider(shared_ptr<BaseCollider> collider);
	void AddRigidbody(shared_ptr<Rigidbody> rigidbody);
	void ApplyForce(shared_ptr<Rigidbody> rigidbody, const Vec3& target, const Vec3& source, float massDifference);
	vector<shared_ptr<BaseCollider>> GetNearbyTargets(const Vec3& playerPosition, float radius);

private:
	void HandleCollision(shared_ptr<BaseCollider> colliderA, shared_ptr<BaseCollider> colliderB);

private:
	vector<shared_ptr<BaseCollider>> _colliders;
	vector<shared_ptr<Rigidbody>> _rigidbodies;
};

