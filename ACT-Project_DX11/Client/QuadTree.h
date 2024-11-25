#pragma once

struct QuadTreeNode {
    int startX, startZ; // ����� ���� ��ǥ
    int size;           // ��尡 ����ϴ� ������ ũ��
    std::vector<int> indices; // �ε��� ������
    QuadTreeNode* children[4] = { nullptr, nullptr, nullptr, nullptr }; // �ڽ� ���
};

class QuadTree {
public:
    QuadTree(const std::vector<VertexTextureNormalTangentData>& vertices, int terrainWidth, int terrainHeight)
        : vertices(vertices), terrainWidth(terrainWidth), terrainHeight(terrainHeight) {}

    void Build(int startX, int startZ, int size);
    void Render(QuadTreeNode* node, const Vec3& cameraPosition, float detailDistance, std::vector<uint32_t>& outIndices);


    QuadTreeNode* GetRoot() { return root; }

    void SetRoot(QuadTreeNode* root) { this->root = root; }

private:
    const std::vector<VertexTextureNormalTangentData>& vertices;
    int terrainWidth;
    int terrainHeight;
    QuadTreeNode* root = nullptr;

    void BuildRecursive(int startX, int startZ, int size, QuadTreeNode*& node);
    void Clear(QuadTreeNode* node);
};
