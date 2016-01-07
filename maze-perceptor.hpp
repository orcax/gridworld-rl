#pragma once
#ifndef __MAZE_HPP
#define __MAZE_HPP

#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

#define ROAD '0'
#define WALL '#'
#define TRAP 'x'
#define GOAL '*'
#define NONE 0
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Cell {
  public:
    int row;
    int col;
    char type;
    int reward;

    Cell() {}

    Cell(int row, int col, char type, int reward) {
      this->row = row;
      this->col = col;
      this->type = type;
      this->reward = reward;
    }

    Cell& operator=(const Cell& other) {
      this->row = other.row;
      this->col = other.col;
      this->type = other.type;
      this->reward = other.reward;
      return *this;
    }

    friend ostream& operator<<(ostream& os, const Cell& other) {
      os << "(" << other.row << "," << other.col << "):" << other.type << "," << other.reward;
      return os;
    }
};

typedef int Action;

class MazePerceptor {

  public:
    MazePerceptor(string path);
    ~MazePerceptor();
    void initMaze(string path);
    void freeMaze();
    void prtMaze();
    vector<int> getActions();
    int getRows();
    int getCols();
    Cell act(const Cell& currCell, int action);
    Cell act(int row, int col, int action);

  private:
    int rows, cols;
    Cell** maze;
    vector<Action> actions;

    int getReward(char type);
};

#endif
