
#include "Maze.h"




Maze::Maze(int size , std::string   mazeAlgoName , bool pathsOrWalls):size(size) , start({-1,-1}) , target({-1,-1}) ,
            mazeArr(size , std::vector<bool>(size , pathsOrWalls)) , mazeAlgoName(std::move(mazeAlgoName)) {}



bool Maze::operator()(const coordinate &coord){
    if(coord.first < 0 || coord.first >= size || coord.second < 0 || coord.second >= size){
        throw std::out_of_range("Index is out of bounds!");
    }
    return mazeArr[coord.first][coord.second];
}

void Maze::operator()(const coordinate &coord, bool newVal) {
    if(coord.first < 0 || coord.first >= size || coord.second < 0 || coord.second >= size){
        throw std::out_of_range("Index is out of bounds!");
    }
    mazeArr[coord.first][coord.second] = newVal;
}

int Maze::getSize() const {
    return size;
}

const coordinate &Maze::getStart() const {
    return start;
}

const std::string& Maze::getMazeAlgoName() const
{
    return mazeAlgoName;
}

void Maze::setStart(const coordinate &newStart) {
    Maze::start = newStart;
}

const coordinate &Maze::getTarget() const {
    return target;
}

void Maze::setTarget(const coordinate &newTarget) {
    Maze::target = newTarget;
}
