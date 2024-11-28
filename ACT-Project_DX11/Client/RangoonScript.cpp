#include "pch.h"
#include "RangoonScript.h"

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
        onTarget = true;
        return; // ��ǥ�� ����
    }
    
    onTarget = false;

    direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

    _transform->SetPosition(_transform->GetPosition() + direction * _speed * _deltaTime);  // ���� �Ÿ���ŭ �̵�
}

void RangoonScript::Rota(const Vec3 targetPos)
{
    CurForward = _transform->GetLook();
    direction = targetPos - _transform->GetPosition();
    direction.Normalize();

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
    if (abs(angle) < 0.001f) // 0.01 ����(�� 0.57��) ���� ȸ�� ����
    {
        return;
    }
    // ���⿡ ���� ���� ���� (y�� �߽� ȸ��)
    if (rotationAxis.y < 0) {
        angle = -angle;  // ���� ȸ��
    }

    // ���� ȸ���� ������Ʈ
    Vec3 currentRotation = _transform->GetLocalRotation();
    Vec3 newRotation = Vec3::Lerp(currentRotation, currentRotation + Vec3(0, angle, 0), 0.1f); // 0.1f�� ���� �ӵ�
    _transform->SetRotation(newRotation);
   
}

void RangoonScript::Tracking(Vec3 pos, const std::vector<Node3D>& path)
{
    if (path.empty()) {
        return;
    }

    // ��� ���� �� ��带 ���� �̵�
    for (size_t i = 0; i < path.size(); ++i) {
        // ���� ��ġ�� ��ǥ ��忡 �����ߴٸ� ���� ���� �̵�
        if (i + 1 < path.size()) {
            Move(path[i + 1].pos);
        }
    }
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

    //Node3D start = { _transform->GetPosition(), 0, 0, nullptr };
    //Node3D goal = { {0,0,0},0,0,nullptr };
    
    //std::vector<Node3D> path = astar.findPath(start, goal);

    //Tracking(_transform->GetPosition(), path);
    //if (INPUT->GetButton(KEY_TYPE::KEY_4))
    {
        Move(playerPosition);
        Rota(playerPosition);
        if (onTarget == true)
        {
            SetAnimationState(AnimationState::Attack1);
        }
        else
        {
            SetAnimationState(AnimationState::Run);
        }
    }
    
}

void RangoonScript::SetAnimationState(AnimationState state)
{
    _modelAnimator->ChangeAnimation(state);
    _currentAnimationState = state;
}