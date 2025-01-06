#include "pch.h"
#include "Octree.h"
#include "BaseCollider.h"

void Octree::Init(const BoundingBox& worldBounds, int maxDepth)
{
    // 기존 루트 노드 제거
    Clear();
    _root = std::make_unique<OctreeNode>(worldBounds, 0, maxDepth);
}

void Octree::UpdateCollider(shared_ptr<BaseCollider> collider)
{
    RemoveCollider(collider); // 기존 노드에서 제거
    InsertCollider(collider); // 새로운 위치로 삽입
}

void Octree::Clear()
{
    // 루트 노드를 초기화하여 메모리 해제
    _root.reset();
}


void Octree::InsertCollider(std::shared_ptr<BaseCollider> collider)
{
    if (_root)
    {
        _root->InsertCollider(collider);
    }
}

void Octree::RemoveCollider(std::shared_ptr<BaseCollider> collider)
{
    if (_root)
    {
        _root->RemoveCollider(collider);
    }
}

std::vector<std::shared_ptr<BaseCollider>> Octree::QueryColliders(const shared_ptr<BaseCollider>& rangeCollider)
{
    if (_root)
    {
        return _root->QueryColliders(rangeCollider);
    }
    return {};
}

std::vector<std::shared_ptr<BaseCollider>> Octree::QueryColliders(const Ray& ray)
{
    if (_root)
    {
        return _root->QueryColliders(ray);
    }
    return {};
}

void Octree::RenderOctree()
{
    if (!(DEBUG->IsDebugEnabled() || INPUT->GetButton(KEY_TYPE::CAPSLOCK)))
        return;

    if (_root)
        _root->RenderNode();
}

size_t Octree::GetTotalColliderCount() const
{
    if (_root)
    {
        return _root->GetTotalColliderCount();
    }
    return 0;
}