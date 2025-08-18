#include "grid.h"
#include <cmath>

Grid::Grid(int rows, int cols) {
    this->rows = rows;
    this->cols = cols;
    this->grid = std::vector<std::vector<Cell>>(rows, std::vector<Cell>(cols));
}

void Grid::createGrid() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c].r = r;
            grid[r][c].c = c;
        }
    }
}

void Grid::clearGrid() {
    for (auto& row : grid) {
        for (auto& cell : row) {
            cell.resetPath();
        }
    }
}

bool Grid::mouseToCell(sf::Vector2f mousePos, int* r, int* c) const {
    *c = mousePos.x / CELL_SIZE;
    *r = mousePos.y / CELL_SIZE;
    return (*c >= 0 && *c < cols && *r >= 0 && *r < rows);
}

void Grid::drawGrid(sf::RenderWindow& window) const{
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
