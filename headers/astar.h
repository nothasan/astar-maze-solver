#pragma once    


#include <vector>
#include <utility>
#include "headers/node.h"  // or wherever your Cell struct is defined

bool aStarStep(std::vector<std::vector<Cell>>& grid, int goal_r, int goal_c);
void aStarInit(std::vector<std::vector<Cell>>& grid, int start_r, int start_c, int goal_r, int goal_c);