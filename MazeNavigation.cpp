#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <random>
#include <climits> // Include for INT_MAX

// Define the structure for the maze cell
struct Cell {
    int x, y;
    bool walkable;
    Cell* parent;
    int g, h;
    Cell(int x, int y, bool walkable) : x(x), y(y), walkable(walkable), parent(nullptr), g(INT_MAX), h(0) {}

    int f() const { return g + h; }

    // Operator to compare cells by their F cost
    bool operator<(const Cell& other) const {
        return f() > other.f(); // Inverted to make the priority_queue a min-heap
    }
};

// Custom comparator for the priority queue
struct CompareCell {
    bool operator()(const Cell* a, const Cell* b) const {
        return *a < *b;
    }
};

// Heuristic function for A* (Manhattan distance)
int heuristic(const Cell& a, const Cell& b) {
    return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

// Function to reconstruct the path from end to start
std::vector<Cell> reconstruct_path(Cell* current) {
    std::vector<Cell> path;
    while (current) {
        path.push_back(*current);
        current = current->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

// A* search algorithm
std::vector<Cell> a_star_search(Cell* start, Cell* end, std::vector<std::vector<Cell>>& grid) {
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> open_set;
    std::unordered_set<Cell*> open_set_items; // To keep track of items in the priority queue
    start->g = 0;
    start->h = heuristic(*start, *end);
    open_set.push(start);
    open_set_items.insert(start);

    while (!open_set.empty()) {
        Cell* current = open_set.top();
        open_set.pop();
        open_set_items.erase(current);

        if (current == end) {
            return reconstruct_path(end);
        }

        // Check all adjacent cells
        static const std::vector<std::pair<int, int>> directions = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // Up, Down, Left, Right
        for (const auto& dir : directions) {
            int new_x = current->x + dir.first;
            int new_y = current->y + dir.second;

            if (new_x >= 0 && new_x < grid.size() && new_y >= 0 && new_y < grid[0].size()) {
                Cell* neighbor = &grid[new_x][new_y];
                if (!neighbor->walkable || open_set_items.find(neighbor) != open_set_items.end()) {
                    continue;
                }

                int tentative_g_score = current->g + 1;
                if (tentative_g_score < neighbor->g) {
                    neighbor->parent = current;
                    neighbor->g = tentative_g_score;
                    neighbor->h = heuristic(*neighbor, *end);
                    open_set.push(neighbor);
                    open_set_items.insert(neighbor);
                }
            }
        }
    }

    return std::vector<Cell>(); // Return an empty path if no path is found
}