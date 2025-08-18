#pragma once    


#include <vector>
#include <utility>
#include "headers/node.h"  // or wherever your Cell struct is defined
#include "grid.h"

bool aStarStep(Grid &grid, int goal_r, int goal_c);
void aStarInit(Grid &grid, int start_r, int start_c, int goal_r, int goal_c);