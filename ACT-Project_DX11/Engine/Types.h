#pragma once
#include <windows.h>
#include "DirectXMath.h"
#include "SimpleMath.h"

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

using Color = DirectX::XMFLOAT4;

using Vec2 = DirectX::SimpleMath::Vector2;
using Vec3 = DirectX::SimpleMath::Vector3;
using Vec4 = DirectX::SimpleMath::Vector4;
using Matrix = DirectX::SimpleMath::Matrix;
using Quaternion = DirectX::SimpleMath::Quaternion;
using Ray = DirectX::SimpleMath::Ray;

// MeshID / MaterialID
using InstanceID = std::pair<uint64, uint64>;

constexpr uint64_t DEFAULT_MESH_ID = 0;       // Mesh가 없는 Collider의 기본 ID
constexpr uint64_t DEFAULT_MATERIAL_ID = 0;  // Material이 없는 Collider의 기본 ID

enum class AnimationState
{
	Idle,
	Walk,
	Run,
	Attack1,
	Attack2,
	Attack3,
	Attack4,
	Jump,
	Hit, //피격
	Die, //죽음
	Roar, //울음,포효
	Aggro,
	// 다른 상태 추가 가능
};
