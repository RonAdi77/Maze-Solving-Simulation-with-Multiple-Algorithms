
#include "Generator.h"



std::shared_ptr<Maze> Generator::generateMaze(int size, int algo)
{

    switch (algo){
    case DFS:
        return dfsGenerator(std::make_shared<Maze>(size , "DFS" , false));
    case PRIM:
        return primsGenerator(std::make_shared<Maze>(size , "PRIM",false));
    case HUNT_AND_KILL:
        return huntAndKillGenerator(std::make_shared<Maze>(size , "HUNT_AND_KILL",false));
    case RECURSIVE_DIVISION:
        return recursiveDivisionGenerator(std::make_shared<Maze>(size , "RECURSIVE_DIVISION",true));
    default:
        break;
    }
    return nullptr;
}


std::shared_ptr<Maze> Generator::dfsGenerator(std::shared_ptr<Maze> maze)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::vector<coordinate> directions = {{0,-1} , {1,0} , {0 , 1} , {-1 , 0}};//up,right,down,left.
    std::stack<coordinate> stack;

    stack.emplace(1,1);
    (*maze)({1 , 1},true);

    while (!stack.empty()){
        coordinate current = stack.top();
        stack.pop();
        std::shuffle(directions.begin() , directions.end() , rng);

        for(coordinate coord : directions){
            coordinate next = current + coord*2;

            if(next.first >= 0 && next.first < maze->getSize()-1 && next.second >= 0 && next.second < maze->getSize()-1 && !(*maze)(next)){
                (*maze)(next,true);
                (*maze)(current + coord,true);
                stack.push(next);
            }
        }
    }
    (*maze)({1 , 0},true);
    (*maze)({maze->getSize()-2 , maze->getSize()-1} , true);
    maze->setStart({1 ,0});
    maze->setTarget({maze->getSize()-2 , maze->getSize()-1});
    return maze;

}

std::shared_ptr<Maze> Generator::primsGenerator(std::shared_ptr<Maze> maze){
    std::vector<coordinate> directions = {{0,-1} , {1,0} , {0 , 1} , {-1 , 0}};
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> startCoordRange(1, maze->getSize()-3);

    std::vector<coordinate> potenCoord;

    coordinate startCoord = {startCoordRange(rng) , startCoordRange(rng)};
    makeItOddCoord(startCoord);
    (*maze)(startCoord , true);

    for(coordinate coord : directions){
        coordinate next = startCoord + coord*2;
        if(next.first >= 0 && next.first < maze->getSize()-1 && next.second >= 0 && next.second < maze->getSize()-1 && !(*maze)(next)){
            potenCoord.push_back(next);
        }
    }

    bool firstNeighborFlag = false;

    while (!potenCoord.empty()){
        std::uniform_int_distribution<int> randomIndexRange(0, static_cast<int>(potenCoord.size()-1));
        int randIndex = randomIndexRange(rng);
        coordinate current = potenCoord.at(randIndex);


        potenCoord.at(randIndex) = potenCoord.at(potenCoord.size()-1);
        potenCoord.pop_back();
        // potenCoord.erase(potenCoord.begin() + randIndex);
        if ((*maze)(current)){continue;}
        (*maze)(current , true);

        std::shuffle( directions.begin() , directions.end() , rng);

        for(coordinate coord : directions){
            coordinate next = current + coord*2;
            if(next.first >= 0 && next.first < maze->getSize()-1 && next.second >= 0 && next.second < maze->getSize()-1){
                if(!(*maze)(next)){
                    potenCoord.push_back(next);
                }
                else{
                    if(!firstNeighborFlag){
                        (*maze)(current + coord,true);
                        firstNeighborFlag = true;
                    }
                }
            }
        }
        firstNeighborFlag = false;
    }
    (*maze)({1 , 0},true);
    (*maze)({maze->getSize()-2 , maze->getSize()-1} , true);
    maze->setStart({1 ,0});
    maze->setTarget({maze->getSize()-2 , maze->getSize()-1});
    return maze;
}

std::shared_ptr<Maze> Generator::huntAndKillGenerator(std::shared_ptr<Maze> maze)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::vector<coordinate> directions = {{0,-1} , {1,0} , {0 , 1} , {-1 , 0}};//up,right,down,left.
    std::vector<coordinate> visited;

    std::uniform_int_distribution<int> startCoordRange(1, maze->getSize()-3);

    coordinate startCoord = {startCoordRange(rng) , startCoordRange(rng)};
    makeItOddCoord(startCoord);
    (*maze)(startCoord , true);
    visited.emplace_back(startCoord);

    coordinate current = startCoord;
    int currentVisitedIndex = 0;

    while (!visited.empty()){
        coordinate tempCoord = current;
        for(coordinate neighbor : directions){
            coordinate next = current + neighbor*2;
            if(next.first >= 0 && next.first < maze->getSize()-1 && next.second >= 0 && next.second < maze->getSize()-1){
                if(!(*maze)(next)){
                    visited.push_back(next);
                    (*maze)(next, true);
                    (*maze)(current + neighbor, true);
                    tempCoord = next;
                    break;
                }
            }
        }
        if (current == tempCoord){
            visited.at(currentVisitedIndex) = visited.at(visited.size()-1);
            visited.pop_back();
            if (visited.empty())break;
            std::uniform_int_distribution<int> randomKillIndex(0, static_cast<int>(visited.size())-1);
            currentVisitedIndex = randomKillIndex(rng);
            current = visited.at(currentVisitedIndex);
        }
    }
    maze->setStart({1,0});
    maze->setTarget({maze->getSize()-2 , maze->getSize()-1});
    (*maze)({maze->getSize()-2 , maze->getSize()-1} , true);
    return maze;

}


std::shared_ptr<Maze> Generator::recursiveDivisionGenerator(std::shared_ptr<Maze> maze){
    enum divisionDirection{HORIZONTAL = 0, VERTICAL = 1};
    for (int i = 0 ; i < maze->getSize() ; i++){
        (*maze)({0,i},false);
        (*maze)({maze->getSize()-1,i},false);
        (*maze)({i,0},false);
        (*maze)({i,maze->getSize()-1},false);
    }

    std::stack<std::pair<coordinate, coordinate>> stack;
    stack.push({{1,1},{maze->getSize()-2 , maze->getSize()-2}});

    while (!stack.empty()){
        std::pair<coordinate,coordinate> stackTop = stack.top();
        stack.pop();
        for (std::pair<coordinate,coordinate> cordPair : recursiveDivisionHelper(maze,stackTop.first,stackTop.second)){
            stack.emplace(cordPair);
        }
    }


    (*maze)({1,0} , true);
    maze->setStart({1,0});
    maze->setTarget({maze->getSize()-2 , maze->getSize()-1});
    (*maze)({maze->getSize()-2 , maze->getSize()-1} , true);
    return maze;
}


std::vector<std::pair<coordinate, coordinate>> Generator::recursiveDivisionHelper(const std::shared_ptr<Maze>& maze, const coordinate& upLeft,const coordinate& bottomRight){
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> chooseDirection(0, 1);

    enum DivisionDirection{HORIZONTAL = 0,VERTICAL = 1};
    DivisionDirection divisionDirection;
    std::vector<std::pair<coordinate,coordinate>> result;
    bool canDivHorizontal = false, canDivVertical= false;
    canDivHorizontal =  (bottomRight.first - upLeft.first + 1 > 4 ) && (bottomRight.second - upLeft.second +1 >=2);
    canDivVertical =  (bottomRight.second - upLeft.second + 1 > 4) && (bottomRight.first - upLeft.first +1 >= 2);

    if (canDivHorizontal && canDivVertical){
        divisionDirection = static_cast<DivisionDirection>(chooseDirection(rng));
    }
    else if ( canDivVertical){
        divisionDirection = VERTICAL;
    }
    else if (canDivHorizontal ){
        divisionDirection = HORIZONTAL;
    }
    else return result;

    if (divisionDirection == HORIZONTAL){
        std::uniform_int_distribution<int> chooseRow(upLeft.first+1,bottomRight.first-1);
        std::uniform_int_distribution<int> chooseCol(upLeft.second,bottomRight.second);
        int row = chooseRow(rng);
        int col = chooseCol(rng);
        while ((*maze)({row,upLeft.second-1}) || (*maze)({row,bottomRight.second+1})){
            row = chooseRow(rng);
        }
        for ( int i = upLeft.second ; i <= bottomRight.second ; i++){
            if ( i != col  ){
                (*maze)({row,i}, false);
            }
        }
        result.push_back({upLeft,{row-1,bottomRight.second}});
        result.push_back({{row+1,upLeft.second},bottomRight});
    }
    if (divisionDirection == VERTICAL){
        std::uniform_int_distribution<int> chooseRow(upLeft.first,bottomRight.first);
        std::uniform_int_distribution<int> chooseCol(upLeft.second+1,bottomRight.second-1);
        int row = chooseRow(rng);
        int col = chooseCol(rng);
        while ((*maze)({upLeft.first-1,col}) || (*maze)({bottomRight.first+1,col})){
            col = chooseCol(rng);
        }
        for ( int i = upLeft.first ; i <= bottomRight.first ; i++){
            if (i != row){
                (*maze)({i,col}, false);
            }
        }
        result.push_back({upLeft,{bottomRight.first,col-1}});
        result.push_back({{upLeft.first,col + 1},bottomRight});
    }
    return result;
}


void Generator::multipleSolutionsGenerator(const std::shared_ptr<Maze>& maze , int wallToRemove){
    enum compass{UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};
    std::vector<coordinate> directions = {{-1,0} , {0,1} , {1 , 0} , {0 , -1}};//up,right,down,left.

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> randCoord(1, maze->getSize()-2);

    while(wallToRemove > 0){
        coordinate current = {randCoord(rng), randCoord(rng)};

        if ((*maze)(current))continue;
        int neighborWallCounter = 0;
        for (int i = 0 ; i < 4 ; i++){
            if (!(*maze)(current + directions[i]))neighborWallCounter++;
        }
        if (neighborWallCounter != 2)continue;
        if ((!(*maze)(current + directions[UP]) && !(*maze)(current + directions[DOWN])) ||
            (!(*maze)(current + directions[LEFT]) && !(*maze)(current + directions[RIGHT]))){
            (*maze)(current, true);
            wallToRemove--;
        }
    }
}





void Generator::makeItOddCoord(coordinate &startCoord) {
    if (startCoord.first % 2 != 0 ){
        startCoord.first += startCoord.first % 2 + 1;
    }
    if (startCoord.second % 2 != 0 ){
        startCoord.second += startCoord.second % 2 + 1;
    }
}


