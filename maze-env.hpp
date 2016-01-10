#pragma once
#ifndef __MAZE_ENV_HPP
#define __MAZE_ENV_HPP

#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>

using namespace std;

#define ROAD '0'
#define WALL '#'
#define GOAL '*'

#define NONE  0
#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

class Cell {

  public:
    int row;
    int col;
    char type;

    Cell() {}

    Cell(int row, int col, char type=ROAD) {
      this->row = row;
      this->col = col;
      this->type = type;
    }

    Cell& operator=(const Cell& other) {
      this->row = other.row;
      this->col = other.col;
      this->type = other.type;
      return *this;
    }

    string str() {
      stringstream ss;
      ss << row << "_" << col;
      return ss.str();
    }

    friend ostream& operator<<(ostream& os, const Cell& other) {
      os << "(" << other.row << "," << other.col << "):" << other.type;
      return os;
    }

};

class MazeEnv {

  public:
    MazeEnv(string path);
    ~MazeEnv();
    void initMaze(string path);
    void freeMaze();
    void start(int row, int col);
    int act(const Cell& cell, int action);
    void valueIterate(float gamma=0.9);
    void prtMaze() const;
    vector<int> getActions() const;
    Cell getCell(int row, int col) const;
    Cell getCurrCell() const;
    int getRows() const;
    int getCols() const;

  private:
    int rows, cols;
    Cell** maze;
    int** rewards;
    double** values;
    Cell currCell;
    vector<int> actions;

    Cell nextCell(const Cell& cell, int action);

};

#endif
