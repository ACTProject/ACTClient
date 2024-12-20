#pragma once
#include "EnginePch.h"

// 3D 노드 구조체
struct Node3D {
    Vec3 pos;           // 위치
    float gCost, hCost; // g: 현재까지 이동 비용, h: 휴리스틱 비용
    float fCost() const { return gCost + hCost; } // 총 비용
    Node3D* parent; // 부모 노드 (경로 추적용)

    // 우선순위 큐에서 사용하기 위한 비교 연산자 (const로 정의)
    bool operator<(const Node3D& other) const {
        return fCost() > other.fCost(); // 비용이 낮은 순서로 정렬
    }
    // == 연산자 추가 (Vec3 비교를 위한)
    bool operator==(const Node3D& other) const {
        return pos == other.pos;  // Vec3의 == 연산자를 사용하여 비교
    }
};

class A_Star {
public:
    A_Star() {}
    std::vector<Node3D> findPath(Node3D start, Node3D goal);

private:
    float heuristic3D(const Node3D& a, const Node3D& b);
    float distance3D(const Node3D& a, const Node3D& b);
    std::vector<Node3D> getNeighbors(const Node3D& node);
    int hash3D(const Node3D& node);
    std::vector<Node3D> reconstructPath(const Node3D& node);
};
