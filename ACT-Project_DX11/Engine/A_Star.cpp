#include "pch.h"
#include "A_Star.h"

std::vector<Node3D> A_Star::findPath(Node3D start, Node3D goal) {
    std::priority_queue<Node3D> openSet;
    std::unordered_map<int, Node3D> closedSet;
    std::vector<Node3D> path;

    start.gCost = 0;
    start.hCost = heuristic3D(start, goal);
    start.parent = nullptr;

    openSet.push(start);

    while (!openSet.empty()) {
        Node3D current = openSet.top();
        openSet.pop();

        if (current.pos.x == goal.pos.x && current.pos.y == goal.pos.y && current.pos.z == goal.pos.z) {
            return reconstructPath(current);
        }

        for (auto& neighbor : getNeighbors(current)) {
            float tentativeGCost = current.gCost + distance3D(current, neighbor);

            if (closedSet.find(hash3D(neighbor)) != closedSet.end() &&
                tentativeGCost >= closedSet[hash3D(neighbor)].gCost) {
                continue;
            }

            neighbor.gCost = tentativeGCost;
            neighbor.hCost = heuristic3D(neighbor, goal);
            neighbor.parent = new Node3D(current);

            openSet.push(neighbor);
            closedSet[hash3D(neighbor)] = neighbor;
        }
    }

    return {};
}

float A_Star::heuristic3D(const Node3D& a, const Node3D& b) {
    return std::sqrt((b.pos.x - a.pos.x) * (b.pos.x - a.pos.x) +
        (b.pos.y - a.pos.y) * (b.pos.y - a.pos.y) +
        (b.pos.z - a.pos.z) * (b.pos.z - a.pos.z));
}

float A_Star::distance3D(const Node3D& a, const Node3D& b) {
    return std::sqrt((b.pos.x - a.pos.x) * (b.pos.x - a.pos.x) +
        (b.pos.y - a.pos.y) * (b.pos.y - a.pos.y) +
        (b.pos.z - a.pos.z) * (b.pos.z - a.pos.z));
}

std::vector<Node3D> A_Star::getNeighbors(const Node3D& node) {
    const int dx[6] = { 1, -1, 0, 0, 0, 0 };
    const int dy[6] = { 0, 0, 1, -1, 0, 0 };
    const int dz[6] = { 0, 0, 0, 0, 1, -1 };

    std::vector<Node3D> neighbors;
    for (int i = 0; i < 6; ++i) {
        Node3D neighbor = { {node.pos.x + dx[i], node.pos.y + dy[i], node.pos.z + dz[i]}, 0, 0, nullptr };
        neighbors.push_back(neighbor);
    }
    return neighbors;
}

int A_Star::hash3D(const Node3D& node) {
    return static_cast<int>(node.pos.x * 10000 + node.pos.y * 100 + node.pos.z);
}

std::vector<Node3D> A_Star::reconstructPath(const Node3D& node) {
    std::vector<Node3D> path;
    const Node3D* current = &node;

    while (current) {
        path.push_back(*current);
        current = current->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}
