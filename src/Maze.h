#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>
#include "common.h"
#include <utility>


class Maze {
public:
    Maze(int , std::string,bool);
    ~Maze() = default;
    bool operator()(const coordinate&);
    void operator()(const coordinate& , bool);
    [[nodiscard]] int getSize() const;
    [[nodiscard]] const coordinate &getStart() const;
    [[nodiscard]] const std::string &getMazeAlgoName() const;
    void setStart(const coordinate&);
    [[nodiscard]] const coordinate &getTarget() const;
    void setTarget(const coordinate&);



private:
    int size;
    coordinate start , target;
    std::vector<std::vector<bool>> mazeArr;
    std::string mazeAlgoName;
};


