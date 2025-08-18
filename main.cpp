#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "headers/globals.h" 
#include "headers/node.h"
#include "headers/astar.h" // Make sure your new functions are declared in the header
#include <iostream>
#include <cmath>

void drawGrid(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& grid);




bool mouseToCell(sf::Vector2f mousePos, int* r, int* c) {
    *c = mousePos.x / CELL_SIZE;
    *r = mousePos.y / CELL_SIZE;
    return (*c >= 0 && *c < COLS && *r >= 0 && *r < ROWS);
}

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
    std::vector<std::vector<Cell>> grid(ROWS, std::vector<Cell>(COLS));
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            grid[r][c].r = r;
            grid[r][c].c = c;
        }
    }

    bool astarActive = false;
    int start_r, start_c, goal_r, goal_c;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
        window.close();

    if (event.type == sf::Event::MouseButtonPressed) {
        int r, c;
        if (mouseToCell(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), &r, &c)) {
            if (startCell == sf::Vector2i(-1, -1)) {
                clearGrid(grid); // Clear the grid when a new start cell is selected
                startCell = sf::Vector2i(c, r);
                grid[r][c].state = CellState::Open;
            } else if (goalCell == sf::Vector2i(-1, -1)) {
                goalCell = sf::Vector2i(c, r);
                grid[r][c].state = CellState::Open;
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
        drawGrid(window, grid);
        window.display();
        }

      
         
    }






void drawGrid(sf::RenderWindow& window, const std::vector<std::vector<Cell>>& grid) {
    static sf::Clock clock;
    float t = clock.getElapsedTime().asSeconds();

    // Checkerboard background
    sf::RectangleShape bg(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            bg.setPosition(c * CELL_SIZE, r * CELL_SIZE);
            bool alt = ((r + c) & 1);
            bg.setFillColor(alt ? sf::Color(18, 18, 24) : sf::Color(22, 22, 30));
            window.draw(bg);
        }
    }

    // Main tile (slightly inset for grid lines)
    sf::Vector2f size(CELL_SIZE - 3.f, CELL_SIZE - 3.f);
    sf::RectangleShape rect(size);
    rect.setOrigin(size.x * 0.5f, size.y * 0.5f);

    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            const Cell& cell = grid[r][c];
            float cx = c * CELL_SIZE + CELL_SIZE * 0.5f;
            float cy = r * CELL_SIZE + CELL_SIZE * 0.5f;

            sf::Color fill;
            float scale = 1.0f;

            switch (cell.state) {
                case CellState::Open: {
                    // Pulse between bright/dim blue
                    float pulse = 0.5f + 0.5f * std::sin(t * 4.f + (r + c) * 0.2f);
                    sf::Uint8 blue = static_cast<sf::Uint8>(150 + 100 * pulse);
                    fill = sf::Color(100, 180, blue);
                    scale = 0.95f + 0.05f * pulse;
                    break;
                }
                case CellState::Closed: {
                    // Fade in with a soft gray/blue
                    float wave = 0.3f + 0.7f * (0.5f + 0.5f * std::sin(t * 2.f + (r + c) * 0.3f));
                    sf::Uint8 shade = static_cast<sf::Uint8>(60 + 60 * wave);
                    fill = sf::Color(shade, shade, 120);
                    break;
                }
                case CellState::Found: {
                    // Glow gold + bounce
                    float bounce = 0.05f * std::sin(t * 8.f + (r + c) * 0.4f);
                    scale = 1.0f + bounce;
                    fill = sf::Color(255, 215, 0);
                    break;
                }
                default: // Unseen
                    fill = sf::Color(30, 30, 40);
                    break;
            }

            // Draw drop shadow
            rect.setPosition(cx + 2.5f, cy + 2.5f);
            rect.setScale(scale, scale);
            rect.setFillColor(sf::Color(0, 0, 0, 80));
            window.draw(rect);

            // Draw main cell
            rect.setPosition(cx, cy);
            rect.setScale(scale, scale);
            rect.setFillColor(fill);
            window.draw(rect);

            // Extra glow for Found + Open
            if (cell.state == CellState::Found || cell.state == CellState::Open) {
                sf::CircleShape glow(CELL_SIZE * 0.6f);
                glow.setOrigin(glow.getRadius(), glow.getRadius());
                glow.setPosition(cx, cy);
                glow.setFillColor(sf::Color(fill.r, fill.g, fill.b, 40));
                glow.setScale(scale * 1.3f, scale * 1.3f);
                window.draw(glow, sf::BlendAdd);
            }
        }
    }
}

