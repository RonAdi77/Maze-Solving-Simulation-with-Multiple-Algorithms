#include "Stat.h"


std::pair<unsigned int , unsigned int > calculate_ni_nde(std::shared_ptr<Maze>& maze){
    std::vector<coordinate> directions = {{0,-1} , {1,0} , {0 , 1} , {-1 , 0}};//up,right,down,left.
    unsigned int ni = 0 ,nde = 0;
    for (int i = 1 ; i < maze->getSize()-1 ;i++){
        for (int j = 1 ; j < maze->getSize()-1 ;j++){
            if ((*maze)({i , j})){
                coordinate current = {i , j};
                int neighborCount = 0;
                for (coordinate cord : directions){
                    coordinate next = current + cord;
                    if(next.first >= 1 && next.first < maze->getSize()-1 && next.second >= 1 && next.second < maze->getSize()-1 && (*maze)(next)){
                        neighborCount++;
                    }
                }
                if (neighborCount > 2)ni++;
                if (neighborCount == 1)nde++;
            }
        }
    }
    return {ni , nde};
}

void generateCsvFile(int sampleNum, int startSize, int sampleSize,int multiple){
    std::ofstream generateFile("multiple_generators.csv", std::ios::trunc);
    std::ofstream solutionFile("multiple_solution.csv",std::ios::trunc);
    if (!generateFile && !solutionFile) return;
    generateFile << "Algo,Multiple,Size,Time,Ni,Nde,Shortest Path\n";
    solutionFile<<"Solution Algo,Maze Algo,Multiple,Size,Time,Nv,Nde,Steps,Path Length,Maze Shortest Path\n";
    for (int i = 0; i < sampleNum; ++i) {
        std::cout <<  i  << " of " << sampleNum << std::endl;
        for (int k = 0; k < sampleSize;k++){
            for (int j = 1; j <= 4; ++j) {
                auto startTime = std::chrono::high_resolution_clock::now(); // start time
                std::shared_ptr<Maze> maze =  Generator::generateMaze(startSize , j);
                Generator::multipleSolutionsGenerator(maze,multiple);
                auto endTime = std::chrono::high_resolution_clock::now(); // end time
                unsigned int generatorDuration = static_cast<unsigned int>(
                        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()
                );
                unsigned int nde,nv,steps,pathLength;
                auto ni_nde = calculate_ni_nde(maze);
                // solution file-----------------------
                startTime = std::chrono::high_resolution_clock::now(); // start time
                bfs(maze,steps,nde,nv,pathLength);
                endTime = std::chrono::high_resolution_clock::now(); // end time
                unsigned int solutionDuration = static_cast<unsigned int>(
                        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()
                );
                solutionFile <<"BFS"<<","<<maze->getMazeAlgoName()<<","<<multiple<<","<<maze->getSize()<<","<<solutionDuration<<","<<nv<<","<<nde<<","<<steps<<","<<pathLength<<","<<pathLength<<"\n";//bfs
                //solution file-----------------------------
                 generateFile <<maze->getMazeAlgoName()<<","<<multiple<<","<<maze->getSize()<<","<<generatorDuration<<","<<ni_nde.first<<","<<ni_nde.second<<","<<pathLength<<"\n";
                unsigned int shortPath = pathLength;
                startTime = std::chrono::high_resolution_clock::now(); // start time
                rightHandRule(maze,steps,nde,nv,pathLength);
                endTime = std::chrono::high_resolution_clock::now(); // end time
                solutionDuration = static_cast<unsigned int>(
                        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count()
                );
                solutionFile <<"RIGHT HAND RULE"<<","<<maze->getMazeAlgoName()<<","<<multiple<<","<<maze->getSize()<<","<<solutionDuration<<","<<nv<<","<<nde<<","<<steps<<","<<pathLength<<","<<shortPath<<"\n";// right hand rule
            }
        }
        startSize += 20;
    }
    generateFile.close();
    solutionFile.close();
}



