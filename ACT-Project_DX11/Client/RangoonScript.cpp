#include "pch.h"
#include "RangoonScript.h"


#include <sstream>

std::string Vec3ToString(const Vec3& vec)
{
    std::ostringstream oss;
    oss << "Vec3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return oss.str();
}

void RangoonScript::Aggro(Vec3& s, Vec3& t)
{

}

void RangoonScript::Move(const Vec3 targetPos)
{
    //y�� ȸ���̴�.
    _transform = GetTransform();
    direction = targetPos - _transform->GetPosition();  // ��ǥ ��ġ ���� ���
    distance = direction.Length();    // ��ǥ���� �Ÿ�
    if (distance < 5.f) {
        return; // ��ǥ�� ����
    }
    direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

    _transform->SetPosition(_transform->GetPosition() + direction * _speed * _deltaTime);  // ���� �Ÿ���ŭ �̵�
}

void RangoonScript::Rota(const Vec3 targetPos)
{
    CurForward = _transform->GetLook();
    if (CurForward.z > 0)
        int a = 0;
    direction = targetPos - _transform->GetPosition();
    direction.Normalize();

    // Vec3�� ���ڿ��� ��ȯ�Ͽ� ���
    std::string forwardStr = Vec3ToString(CurForward);
    std::string directionStr = Vec3ToString(direction);



    // ������ �̿��� ȸ�� �� ���
    Vec3 rotationAxis = CurForward.Cross(direction);

    // ���� ����� �ſ� ������ ���� ���̰� ���� �����Ƿ� ȸ�� �ʿ� ����
    if (rotationAxis.LengthSquared() < 1e-6f)
    {
        return;
    }

    rotationAxis.Normalize();

    // ���� ���
    float angle = std::acos(CurForward.Dot(direction));

    // ���� ������ ����
    if (abs(angle) < 0.01f) // 0.01 ����(�� 0.57��) ���� ȸ�� ����
    {
        return;
    }
    // ����� �޽��� ���
    OutputDebugStringA(("CurForward: " + forwardStr + "\n").c_str());
    OutputDebugStringA(("Direction: " + directionStr + "\n").c_str());
    // ���⿡ ���� ���� ���� (y�� �߽� ȸ��)
    if (rotationAxis.y < 0) {
        angle = -angle;  // ���� ȸ��
    }

        // ���� ȸ���� ������Ʈ
    Vec3 currentRotation = _transform->GetRotation();
    _transform->SetRotation(Vec3(0, currentRotation.y, 0) + Vec3(0, angle, 0));
   
}

void RangoonScript::Tracking(Vec3 pos, const std::vector<Node3D>& path)
{
    //if (path.empty()) {
    //    return;
    //}

    //// ��� ���� �� ��带 ���� �̵�
    //for (size_t i = 0; i < path.size(); ++i) {
    //    // ���� ��ġ�� ��ǥ ��忡 �����ߴٸ� ���� ���� �̵�
    //    if (i + 1 < path.size()) {
    //        Move(pos, path[i + 1].pos);
    //    }
    //}
}




void RangoonScript::Attack()
{

}

void RangoonScript::Start()
{

}

void RangoonScript::Update()
{
    _transform = GetTransform();
    _player = SCENE->GetCurrentScene()->GetPlayer();
    Vec3 playerPosition = _player->GetTransform()->GetPosition();

    //Node3D start = { enemy->GetOrAddTransform()->GetPosition(), 0, 0, nullptr };
    //Node3D goal = { {0,0,0},0,0,nullptr };
    
    //std::vector<Node3D> path = astar.findPath(start, goal);

    //Tracking(enemy->GetTransform()->GetPosition(), path);
    //if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        Move(playerPosition);
        Rota(playerPosition);
    }
    
}

