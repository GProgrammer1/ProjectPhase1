#include <random>
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

