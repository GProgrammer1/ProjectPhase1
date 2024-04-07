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

#include <iostream>
#include <vector>
#include <chrono> // Include for std::chrono

class MazeGenerator {
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> maze;
    std::pair<int, int> start;
    std::pair<int, int> end;
    std::mt19937 rng; // Random number generator

    // Directions for adding walls
    const std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    void addWalls(int row, int col, std::vector<std::pair<int, int>>& walls) {
        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;
            if (isValidCell(newRow, newCol) && maze[newRow][newCol] == 1) {
                walls.emplace_back(newRow, newCol);
            }
        }
    }

    bool isValidCell(int row, int col) const {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    bool isValidWall(int row, int col) const {
        int count = 0;
        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;
            if (isValidCell(newRow, newCol) && maze[newRow][newCol] == 0) {
                count++;
            }
        }
        return count == 1;
    }

public:
    // Constructor with a default seed based on the current time
    MazeGenerator(int r, int c, unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count())
        : rows(r), cols(c), maze(rows, std::vector<int>(cols, 1)), rng(seed) {}

    void generateMaze() {
        std::uniform_int_distribution<int> dist(0, rows - 1), dist2(0, cols - 1);

        // Choose a random starting cell
        start = {dist(rng), dist2(rng)};
        maze[start.first][start.second] = 0;

        std::vector<std::pair<int, int>> walls;
        addWalls(start.first, start.second, walls);

        while (!walls.empty()) {
            std::shuffle(walls.begin(), walls.end(), rng);
            std::pair<int, int> wall = walls.back();
            walls.pop_back();

            int row = wall.first;
            int col = wall.second;

            if (isValidWall(row, col)) {
                maze[row][col] = 0;
                addWalls(row, col, walls);
            }
        }

        // Set the end point opposite from the start
        end = {rows - 1 - start.first, cols - 1 - start.second};
        maze[end.first][end.second] = 0;
    }

    void printMaze() const {
        for (const auto& row : maze) {
            for (int cell : row) {
                std::cout << (cell ? '#' : ' ');
            }
            std::cout << '\n';
        }
    }

    bool isCellWalkable(int x, int y) const {
        return isValidCell(x, y) && maze[x][y] == 0;
    }

    std::pair<int, int> getStart() const { return start; }
    std::pair<int, int> getEnd() const { return end; }
};


// Main function
int main() {
    int rows; // Set the number of rows for the maze
    int cols; // Set the number of columns for the maze

     // Prompt the user for the size of the maze
    std::cout << "Enter the number of rows for the maze: ";
    std::cin >> rows;
    std::cout << "Enter the number of columns for the maze: ";
    std::cin >> cols;

    MazeGenerator generator(rows, cols);
    generator.generateMaze();
    generator.printMaze();
    // Get the start and end points
    auto startCoords = generator.getStart();
    auto endCoords = generator.getEnd();

    // Print the start and end points
    std::cout << "Start: (" << startCoords.first << ", " << startCoords.second << ")\n";
    std::cout << "End: (" << endCoords.first << ", " << endCoords.second << ")\n";

    // The start and end points will be determined by the MazeGenerator
    //auto startCoords = generator.getStart();
    //auto endCoords = generator.getEnd();

    // After generating the maze, create the grid for A*
    std::vector<std::vector<Cell>> grid(rows, std::vector<Cell>(cols, Cell(0, 0, false)));
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            grid[x][y] = Cell(x, y, generator.isCellWalkable(x, y));
        }
    }

    // Define start and end points
    Cell* start = &grid[startCoords.first][startCoords.second];
    Cell* end = &grid[endCoords.first][endCoords.second];

    // Run A* search
    std::vector<Cell> path = a_star_search(start, end, grid);

    // Output the path
    for (const Cell& cell : path) {
        std::cout << "(" << cell.x << ", " << cell.y << ") ";
    }

    return 0;
}
