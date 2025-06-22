cmake_minimum_required(VERSION 3.16)
project(wxWidgetsExample)

set(CMAKE_CXX_STANDARD 20)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

# Include and link wxWidgets
include(${wxWidgets_USE_FILE})
add_executable(wxWidgetsExample main.cpp
        Generator.cpp
        Generator.h
        Maze.cpp
        Maze.h
        Solutions.cpp
        Solutions.h
        Stat.cpp
        Stat.h
        common.h

)
target_link_libraries(wxWidgetsExample ${wxWidgets_LIBRARIES})







