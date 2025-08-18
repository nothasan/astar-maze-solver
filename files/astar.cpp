#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "headers/globals.h" 
#include "headers/node.h"
#include <cmath>
#include "headers/astar.h"
#include <iostream>

static double heuristic(int r1, int c1, int r2, int c2) {
    int dr = r1 - r2;
    int dc = c1 - c2;
    return std::sqrt(dr * dr + dc * dc);
}


static inline std::vector<std::pair<int, int>> getNeighbors() {
    return {
        {-1,  0}, // North
        {-1,  1}, // North-East
        { 0,  1}, // East
        { 1,  1}, // South-East
        { 1,  0}, // South
        { 1, -1}, // South-West
        { 0, -1}, // West
        {-1, -1}  // North-West
    };
}


static double stepCost(const Cell* a, const Cell* b) {
    int dr = std::abs(a->r - b->r);
    int dc = std::abs(a->c - b->c);

    if (dr == 1 && dc == 1) {
        // Diagonal move
        return std::sqrt(2.0);
    } else {
        // Straight move
        return 1.0;
    }
}


static std::vector<std::pair<int,int>> reconstructPath(const std::vector<std::vector<Cell>>& grid, const Cell* goalCell) {
    std::vector<std::pair<int,int>> path;
    int r = goalCell->r, c = goalCell->c;
    while (r != -1 && c != -1) {
        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) break; // Prevent out-of-bounds
        path.emplace_back(r, c);
        const Cell& cur = grid[r][c];
        if (cur.parent_r == -1 && cur.parent_c == -1) break;
        r = cur.parent_r;
        c = cur.parent_c;
    }
    std::reverse(path.begin(), path.end());
    return path;
}


// Static variables to hold A* state
static std::vector<Cell*> openList;
static std::vector<Cell*> closedList;
static Cell* currentCell = nullptr;
static bool astarActive = false;
static std::vector<std::pair<int, int>> path;

// Call this to initialize A* (set start/goal, clear lists)
void aStarInit(std::vector<std::vector<Cell>>& grid, int start_r, int start_c, int goal_r, int goal_c) {
    openList.clear();
    closedList.clear();
    path.clear();
    astarActive = true;
    currentCell = &grid[start_r][start_c];
    currentCell->g = 0.0;
    currentCell->h = heuristic(start_r, start_c, goal_r, goal_c);
    currentCell->f = currentCell->g + currentCell->h;
    openList.push_back(currentCell);
}

// Call this once per frame to advance the algorithm
bool aStarStep(std::vector<std::vector<Cell>>& grid, int goal_r, int goal_c) {
    if (!astarActive || openList.empty()) return false;

   auto currentIt = std::min_element(openList.begin(), openList.end(),
    [](Cell* a, Cell* b) {
        if (a->f == b->f) {
            // Break ties: prefer the node with larger g (deeper into the path)
            return a->g > b->g;
        }
        return a->f < b->f;  // normal A*: pick smaller f
    });
    currentCell = *currentIt;

    if (currentCell->r == goal_r && currentCell->c == goal_c) {
        path = reconstructPath(grid, currentCell);
     
        for (const auto& [r, c] : path)
            grid[r][c].state = CellState::Found;
        astarActive = false;
        return true; // Path found
    } else {
        closedList.push_back(currentCell);
        openList.erase(currentIt);

        for (const auto& [dr, dc] : getNeighbors()) {
            int neighbor_r = currentCell->r + dr;
            int neighbor_c = currentCell->c + dc;
            if (neighbor_r < 0 || neighbor_r >= ROWS || neighbor_c < 0 || neighbor_c >= COLS)
                continue;
            Cell& neighbor = grid[neighbor_r][neighbor_c];
            
            if (std::find(closedList.begin(), closedList.end(), &neighbor) != closedList.end())
                continue;
            double tentative_g = currentCell->g + stepCost(currentCell, &neighbor);
            if (tentative_g < neighbor.g) {
                neighbor.parent_r = currentCell->r;
                neighbor.parent_c = currentCell->c;
                neighbor.g = tentative_g;
                neighbor.h = heuristic(neighbor.r, neighbor.c, goal_r, goal_c);
                neighbor.f = neighbor.g + neighbor.h;
                if (std::find(openList.begin(), openList.end(), &neighbor) == openList.end()) {
                    openList.push_back(&neighbor);
                    neighbor.state = CellState::Open;
                }
            }
        }
    }
    return false; // Not finished yet
}