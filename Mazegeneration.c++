#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

class MazeGenerator {
private:
    int rows;
    int cols;
    std::vector<std::vector<int>> maze;

public:
    MazeGenerator(int rows, int cols) : rows(rows), cols(cols) {
        maze.resize(rows, std::vector<int>(cols, 1));
    }

    void generateMaze() {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> dist(0, 1);

        // Choose a random starting cell
        int startRow = dist(rng) ? 0 : rows - 1;
        int startCol = dist(rng) ? 0 : cols - 1;
        maze[startRow][startCol] = 0;

        std::vector<std::pair<int, int>> walls;
        addWalls(startRow, startCol, walls);

        while (!walls.empty()) {
            std::uniform_int_distribution<int> wallDist(0, walls.size() - 1);
            int wallIndex = wallDist(rng);
            int row = walls[wallIndex].first;
            int col = walls[wallIndex].second;

            if (isValidWall(row, col)) {
                maze[row][col] = 0;
                addWalls(row, col, walls);
            }

            walls.erase(walls.begin() + wallIndex);
        }

        // Set the end point
        int endRow = startRow == 0 ? rows - 1 : 0;
        int endCol = startCol == 0 ? cols - 1 : 0;
        maze[endRow][endCol] = 0;
    }

    void printMaze() {
        for (const auto& row : maze) {
            for (int cell : row) {
                std::cout << (cell ? "#" : " ");
            }
            std::cout << std::endl;
        }
    }

private:
    void addWalls(int row, int col, std::vector<std::pair<int, int>>& walls) {
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(directions.begin(), directions.end(), rng);

        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;

            if (isValidCell(newRow, newCol) && maze[newRow][newCol] == 1) {
                walls.emplace_back(newRow, newCol);
            }
        }
    }

    bool isValidCell(int row, int col) {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    bool isValidWall(int row, int col) {
        int count = 0;
        std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

        for (const auto& dir : directions) {
            int newRow = row + dir.first;
            int newCol = col + dir.second;

            if (isValidCell(newRow, newCol) && maze[newRow][newCol] == 0) {
                count++;
            }
        }

        return count == 1;
    }
};

int main() {
    int rows = 10;
    int cols = 20;

    MazeGenerator generator(rows, cols);
    generator.generateMaze();
    generator.printMaze();

    return 0;
}