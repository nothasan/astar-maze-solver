#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "headers/node.h"
#include "headers/astar.h" 
#include "headers/grid.h"
#include <iostream>
#include <cmath>
#include "headers/globals.h"






void clearGrid(std::vector<std::vector<Cell>>& grid) {
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.resetPath();
        }
    }
}


int main() {

    sf::Vector2i startCell(-1, -1);
    sf::Vector2i goalCell(-1, -1);
    std::vector<std::pair<int, int>> path;
    sf::RenderWindow window(sf::VideoMode(COLS * CELL_SIZE, ROWS * CELL_SIZE), "SFML Grid");
    Grid grid(ROWS, COLS);
    grid.createGrid();

    bool astarActive = false;
    int start_r, start_c, goal_r, goal_c;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
        window.close();

    if (event.type == sf::Event::MouseButtonPressed) {
        int r, c;
        if (grid.mouseToCell(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), &r, &c)) {
            if (startCell == sf::Vector2i(-1, -1)) {
                grid.clearGrid(); // Clear the grid when a new start cell is selected
                startCell = sf::Vector2i(c, r);
                grid.cell(r,c).state = CellState::Open;
            } else if (goalCell == sf::Vector2i(-1, -1)) {
                goalCell = sf::Vector2i(c, r);
                grid.cell(r,c).state = CellState::Open;
            }
        }
    }
}
      // When both start and goal are selected:
      if (startCell != sf::Vector2i(-1, -1) && goalCell != sf::Vector2i(-1, -1) && !astarActive) {
                start_r = startCell.y;
                start_c = startCell.x;
                goal_r = goalCell.y;
                goal_c = goalCell.x;
                aStarInit(grid, start_r, start_c, goal_r, goal_c);
                astarActive = true;
            }

              // Step-by-step A* visualization
        if (astarActive) {
            bool finished = aStarStep(grid, goal_r, goal_c);
            if (finished) {
                astarActive = false;
                startCell = goalCell = sf::Vector2i(-1, -1);
            }
        }
        window.clear();          
        grid.drawGrid(window);
        window.display();
        }

      
         
    }






