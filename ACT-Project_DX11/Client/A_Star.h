#include "EnginePch.h"

// 3D ��� ����ü
struct Node3D {
    Vec3 pos;
    float gCost, hCost; // g: ������� �̵� ���, h: �޸���ƽ ���
    float fCost() const { return gCost + hCost; } // �� ���
    Node3D* parent; // �θ� ��� (��� ������)

    // �켱���� ť���� ����ϱ� ���� �� ������
    bool operator<(const Node3D& other) const {
        return fCost() > other.fCost(); // ����� ���� ������ ����
    }
};

class A_Star {
public:
    A_Star();
    std::vector<Node3D> findPath(Node3D start, Node3D goal);

private:
    float heuristic3D(const Node3D& a, const Node3D& b);
    float distance3D(const Node3D& a, const Node3D& b);
    std::vector<Node3D> getNeighbors(const Node3D& node);
    int hash3D(const Node3D& node);
    std::vector<Node3D> reconstructPath(const Node3D& node);
};