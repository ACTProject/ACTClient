#include "pch.h"
#include "WeaponScript.h"
#include "ModelMesh.h"
#include "Model.h"
#include "Transform.h"

void WeaponScript::Update()
{
	AttachWeaponToHand();
}

void WeaponScript::Start()
{
	
}

void WeaponScript::AttachWeaponToHand()
{
	//_transform = GetTransform();
	//// ���⸦ ������ ��
	//shared_ptr<ModelBone> handBone = _player->GetBoneByName(L"Hand_Grip_L");

	//Matrix handBoneMatrix = handBone->transform;	// �� ���
	//Vec3 handPosition = handBoneMatrix.Translation();	// �� ��ġ
	//Quaternion handQuaternion = Quaternion::CreateFromRotationMatrix(handBoneMatrix);
	//Vec3 handRotation = Transform::ToEulerAngles(handQuaternion); // �� ȸ��

	//_transform->SetPosition(handPosition);
	//_transform->SetRotation(handRotation);
}
