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