#include "pch.h"
#include "RangoonScript.h"



void RangoonScript::Aggro(Vec3& s, Vec3& t)
{

}

void RangoonScript::Move(const Vec3& targetPos)
{
    _transform = GetTransform();

    Vec3 direction = targetPos - _transform->GetPosition();  // ��ǥ ��ġ ���� ���
    float distance = direction.Length();    // ��ǥ���� �Ÿ�
    direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

    // ��ǥ�� �����߰ų� ��������� ��ǥ�� ��Ȯ�� ����
    if (distance < _speed * _deltaTime) {
        _transform->SetPosition(targetPos); // ��ǥ�� ����
    }
    else {
        _transform->SetPosition(_transform->GetPosition() + direction * _speed * _deltaTime);  // ���� �Ÿ���ŭ �̵�
    }
}
//void RangoonScript::Tracking(Vec3 pos, const std::vector<Node3D>& path)
//{
//    if (path.empty()) {
//        return;
//    }
//
//    // ��� ���� �� ��带 ���� �̵�
//    for (size_t i = 0; i < path.size(); ++i) {
//        // ���� ��ġ�� ��ǥ ��忡 �����ߴٸ� ���� ���� �̵�
//        if (i + 1 < path.size()) {
//            Move(pos, path[i + 1].pos);
//        }
//    }
//
//}

void RangoonScript::Attack()
{

}

void RangoonScript::Start()
{

}

void RangoonScript::Update()
{
    //Node3D start = { enemy->GetOrAddTransform()->GetPosition(), 0, 0, nullptr };
    //Node3D goal = { {0,0,0},0,0,nullptr };
    
    //std::vector<Node3D> path = astar.findPath(start, goal);

    //Tracking(enemy->GetTransform()->GetPosition(), path);
    Move({ 0,0,0 });
}