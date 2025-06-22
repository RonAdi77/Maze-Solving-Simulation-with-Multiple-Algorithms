#pragma once

#include <vector>

#define DFS 1
#define PRIM 2
#define HUNT_AND_KILL 3
#define RECURSIVE_DIVISION 4

typedef std::pair<int , int> coordinate;
inline coordinate operator+(const coordinate & lhs, const coordinate & rhs) {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}
inline coordinate operator*(const coordinate & lhs, const int& rhs) {
    return {lhs.first * rhs, lhs.second * rhs};
}
inline bool operator!=(const coordinate & lhs, const coordinate & rhs){
    return lhs.first != rhs.first || lhs.second != rhs.second;
}
inline bool operator==(const coordinate & lhs, const coordinate & rhs){
    return lhs.first == rhs.first && lhs.second == rhs.second;
}
inline void operator+=(coordinate & lhs, const coordinate & rhs){
    lhs.first += rhs.first;
    lhs.second += rhs.second;
}
