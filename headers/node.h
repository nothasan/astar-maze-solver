#pragma once
#include <limits>
#include <cstdint>

enum class CellState : uint8_t { Unseen=0, Open=1, Closed=2, Found = 3 };


struct Cell {

    int r,c; // row and column indices
    bool blocked = false;

    double g = std::numeric_limits<double>::infinity(); // cost from start
    double h = 0.0;                                      // heuristic to goal
    double f = std::numeric_limits<double>::infinity(); // g + h


    int parent_r = -1; // row index of parent cell
    int parent_c = -1; // column index of parent cell


    CellState state = CellState::Unseen;


    void resetPath() {
        g = std::numeric_limits<double>::infinity();
        h = 0.0;
        f = std::numeric_limits<double>::infinity();
        parent_r = parent_c = -1;
        state = CellState::Unseen;
    }

 void resetPath2() {
        g = std::numeric_limits<double>::infinity();
        h = 0.0;
        f = std::numeric_limits<double>::infinity();
        parent_r = parent_c = -1;
        state = CellState::Open;
    }
};