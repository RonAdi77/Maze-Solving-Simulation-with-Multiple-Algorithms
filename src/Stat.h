#pragma once
#include "Solutions.h"
#include <fstream>
#include <chrono>
#include <iostream>

std::pair<unsigned int , unsigned int > calculate_ni_nde(std::shared_ptr<Maze>&);
void generateCsvFile(int , int,int,int);




