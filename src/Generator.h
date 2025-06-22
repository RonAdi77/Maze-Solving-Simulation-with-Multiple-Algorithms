#pragma once
#include "Maze.h"

#include <stack>
#include <random>
#include <algorithm>



class Generator {
public:
    Generator() = default;
    [[nodiscard]] static std::shared_ptr<Maze> generateMaze(int,int);
    static void multipleSolutionsGenerator(const std::shared_ptr<Maze>& , int);

private:
    [[nodiscard]] static std::shared_ptr<Maze> dfsGenerator(std::shared_ptr<Maze>);
    [[nodiscard]] static std::shared_ptr<Maze> primsGenerator(std::shared_ptr<Maze>);

    static std::shared_ptr<Maze> huntAndKillGenerator(std::shared_ptr<Maze>);
    static std::shared_ptr<Maze> recursiveDivisionGenerator(std::shared_ptr<Maze>);
    static void makeItOddCoord(coordinate&);
    static bool checkLegalCoord(int,const coordinate&);
    static std::vector<std::pair<coordinate, coordinate>> recursiveDivisionHelper(const std::shared_ptr<Maze>&,const coordinate&,const coordinate&);
    // std::shared_ptr<Maze> maze;

};



