#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "node.h"
#include "globals.h"

class Grid {
public:
    Grid(int rows, int cols);   // constructor

    void createGrid();          // fill in r/c values
    void clearGrid();           // reset all cells
    bool mouseToCell(sf::Vector2f mousePos, int* r, int* c) const;
    void drawGrid(sf::RenderWindow& window) const;
    Cell& cell(int r, int c) { return grid[r][c]; }

private:
    int rows, cols;
    std::vector<std::vector<Cell>> grid;
};