
#include "Solutions.h"


//steps = number of all the steps the algo do.
//nde = number of dead end.
//nv = number of visited cells.
std::vector<coordinate> rightHandRule(std::shared_ptr<Maze>& maze,unsigned int& steps,unsigned int& nde,unsigned int& nv,unsigned int& pathLength)
{
    enum compass{UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};
    std::vector<coordinate> directions = {{-1,0} , {0,1} , {1 , 0} , {0 , -1}};//up,right,down,left.
    std::vector<coordinate> path;
    std::set<coordinate> visited;

    coordinate currentLocation = maze->getStart();
    compass currentDirection = RIGHT;
    path.push_back(maze->getStart());
    visited.insert(maze->getStart());

    //for statistics
    steps = 0;
    nde = 0;
    unsigned int neighborCount = 0;

    while (currentLocation != maze->getTarget()){

        //for statistics------------------------------------------------------------------------------------------------------------------
        neighborCount = 0;
        for (coordinate cord : directions){
            coordinate next = currentLocation + cord;
            if(next.first >= 1 && next.first < maze->getSize()-1 && next.second >= 1 && next.second < maze->getSize()-1 && (*maze)(next)){
                neighborCount++;
            }
        }
        if (neighborCount == 1)nde++;
        //----------------------------------------------------------------------------------------------------------------------------------

        int rightDirection = (currentDirection + 1) % 4;//calculate the coordinate of the right direction
        if ((*maze)(currentLocation + directions[rightDirection])){//check if there is a turn to the right.
            currentLocation += directions[rightDirection];
            currentDirection = static_cast<compass>(rightDirection);
        }
        else if ((*maze)(currentLocation + directions[currentDirection])){//check if it can continue in the same direction.
            currentLocation += directions[currentDirection];
        }
        else{//flip the direction by 180 degree.
            currentDirection = static_cast<compass>((currentDirection + 2) % 4);
            continue;
        }
        //for statistics
        steps++;

        unsigned long x = visited.size();
        visited.insert(currentLocation);
        if(x == visited.size()){
            while(path[path.size()- 1] != currentLocation){
                path.erase(path.end()-1);
            }
            continue;
        }
        path.push_back(currentLocation);
    }
    //for statistics
    nv = static_cast<int>(visited.size());
    pathLength = path.size();

    return path;
}

std::vector<coordinate> bfs(std::shared_ptr<Maze>& maze,unsigned int& steps,unsigned int& nde,unsigned int& nv,unsigned int& pathLength){
    std::random_device rd;
    std::mt19937 rng(rd());

    std::queue<coordinate> queue;
    std::vector<std::vector<coordinate>> parents(maze->getSize() , std::vector<coordinate>(maze->getSize(), {-1,-1}));
    std::vector<std::vector<bool>> visited(maze->getSize(), std::vector<bool>(maze->getSize(), false));
    std::vector<coordinate> directions = {{-1,0} , {0,1} , {1 , 0} , {0 , -1}};//up,right,down,left.

    //for statistics
    steps = 0;
    nde = 0;
    nv = 1;
    unsigned int neighborCount = 0;

    queue.push(maze->getStart());
    visited[maze->getStart().first][maze->getStart().second] = true;

    while(!queue.empty() && queue.front() != maze->getTarget()){
        coordinate current = queue.front();

        neighborCount = 0;
        for (coordinate cord : directions){
            coordinate next = current + cord;
            if(next.first >= 1 && next.first < maze->getSize()-1 && next.second >= 1 && next.second < maze->getSize()-1 && (*maze)(next)){
                neighborCount++;
            }
        }
        if (neighborCount == 1)nde++;

        std::shuffle(directions.begin() , directions.end() , rng);
        for(coordinate coord : directions){
            coordinate next = current + coord;
            if(next.first >= 0 && next.first < maze->getSize() && next.second >= 0 && next.second < maze->getSize() && (*maze)(next)){
                if (!visited[next.first][next.second]){
                    visited[next.first][next.second] = true;
                    parents[next.first][next.second] = current;
                    queue.push(next);
                    nv++;
                }
            }
        }
        queue.pop();
    }
    std::vector<coordinate> path;
    coordinate currentLocation = maze->getTarget();
    path.push_back(currentLocation);
    while (currentLocation != maze->getStart()){
        path.push_back(parents[currentLocation.first][currentLocation.second]);
        currentLocation = parents[currentLocation.first][currentLocation.second];
    }
    std::ranges::reverse(path);

    //for statistics.
    pathLength = path.size();
    steps = nv -1;

    return path;
}


