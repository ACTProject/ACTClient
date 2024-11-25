#include "pch.h"
#include "QuadTree.h"

void QuadTree::Build(int startX, int startZ, int size) {
    root = new QuadTreeNode();
    BuildRecursive(startX, startZ, size, root);
}

void QuadTree::Render(QuadTreeNode* node, const Vec3& cameraPosition, float detailDistance, std::vector<uint32>& outIndices) {
    if (!node) return;

    // ��� �߽ɰ� ī�޶� �� �Ÿ� ���
    Vec3 nodeCenter = Vec3(node->startX + node->size / 2.0f, 0, node->startZ + node->size / 2.0f);
    float distance = (cameraPosition - nodeCenter).Length();

    // ������ �Ÿ����� ����� ��� ���� ����� �ε����� ���
    if (distance <= detailDistance || node->children[0] == nullptr) {
        // ���� ����� �ﰢ�� �ε����� �߰�
        outIndices.insert(outIndices.end(), node->indices.begin(), node->indices.end());
    }
    else {
        // �ڽ� ��� Ž��
        for (int i = 0; i < 4; ++i) {
            Render(node->children[i], cameraPosition, detailDistance, outIndices);
        }
    }

}

void QuadTree::BuildRecursive(int startX, int startZ, int size, QuadTreeNode*& node)
{
    node->startX = startX;
    node->startZ = startZ;
    node->size = size;

    if (size == 1) {
        // Leaf ���: �ﰢ�� �ε��� ����
        int topLeft = startZ * terrainWidth + startX;
        int topRight = startZ * terrainWidth + (startX + 1);
        int bottomLeft = (startZ + 1) * terrainWidth + startX;
        int bottomRight = (startZ + 1) * terrainWidth + (startX + 1);

        node->indices.push_back(topLeft);
        node->indices.push_back(bottomLeft);
        node->indices.push_back(topRight);

        node->indices.push_back(topRight);
        node->indices.push_back(bottomLeft);
        node->indices.push_back(bottomRight);
    }
    else {
        // ���� ���: �ڽ� ��� ����
        int halfSize = size / 2;

        node->children[0] = new QuadTreeNode();
        BuildRecursive(startX, startZ, halfSize, node->children[0]);

        node->children[1] = new QuadTreeNode();
        BuildRecursive(startX + halfSize, startZ, halfSize, node->children[1]);

        node->children[2] = new QuadTreeNode();
        BuildRecursive(startX, startZ + halfSize, halfSize, node->children[2]);

        node->children[3] = new QuadTreeNode();
        BuildRecursive(startX + halfSize, startZ + halfSize, halfSize, node->children[3]);
    }
}

void QuadTree::Clear(QuadTreeNode* node)
{
    if (!node) return;

    for (int i = 0; i < 4; ++i) {
        if (node->children[i]) {
            Clear(node->children[i]);
            delete node->children[i];
            node->children[i] = nullptr;
        }
    }
    delete node;//
}
