#include "pch.h"
#include "Rangoon.h"



void Rangoon::Aggro(Vec3& s, Vec3& t)
{

}

void Rangoon::Move(Vec3& pos, const Vec3& targetPos)
{
    Vec3 direction = targetPos - pos;  // ��ǥ ��ġ ���� ���
    float distance = direction.Length();    // ��ǥ���� �Ÿ�
    direction.Normalize();  // ���� ���͸� ���� ���ͷ� ����ȭ

    // ��ǥ�� �����߰ų� ��������� ��ǥ�� ��Ȯ�� ����
    if (distance < _speed * _deltaTime) {
        pos = targetPos; // ��ǥ�� ����
    }
    else {
        pos += direction * _speed * _deltaTime;  // ���� �Ÿ���ŭ �̵�
    }
}
void Rangoon::Tracking(Vec3 pos, const std::vector<Node3D>& path)
{
    if (path.empty()) {
        return;
    }

    // ��� ���� �� ��带 ���� �̵�
    for (size_t i = 0; i < path.size(); ++i) {
        // ���� ��ġ�� ��ǥ ��忡 �����ߴٸ� ���� ���� �̵�
        if (i + 1 < path.size()) {
            Move(pos, path[i + 1].pos);
        }
    }

}

void Rangoon::Attack()
{

}

void Rangoon::Start()
{

}

void Rangoon::Update()
{
    Node3D start = { enemy->GetOrAddTransform()->GetPosition(), 0, 0, nullptr };
    Node3D goal = { {0,0,0},0,0,nullptr };
    
    std::vector<Node3D> path = astar.findPath(start, goal);

    Tracking(enemy->GetTransform()->GetPosition(), path);
}