#pragma once
#include "Component.h"
class Rigidbody : public Component
{
public:
	Rigidbody();
	virtual ~Rigidbody();

	void Addforce(const Vec3& force);

	void SetMass(float mass) { _mass = mass; }
	const float GetMass() { return _mass; }
private:
	Vec3 _velocity;		// �ӵ�
	Vec3 _force;		// �ܺ� ��
	float _mass;		// ����
	bool _useGravity	// �߷� ��� ����
};

