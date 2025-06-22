
#pragma once
#include "Generator.h"
#include <queue>
#include <set>

std::vector<coordinate> rightHandRule(std::shared_ptr<Maze>&,unsigned int& ,unsigned int& ,unsigned int&,unsigned int&);
std::vector<coordinate> bfs(std::shared_ptr<Maze>&,unsigned int& ,unsigned int& ,unsigned int&,unsigned int&);


