#include "maze-env.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

MazeEnv::MazeEnv(string path) {
  initMaze(path);
  //actions.push_back(NONE);
  actions.push_back(NORTH);
  actions.push_back(SOUTH);
  actions.push_back(WEST);
  actions.push_back(EAST);
}

MazeEnv::~MazeEnv() {
  freeMaze();
}

void MazeEnv::initMaze(string path) {
  maze = NULL;
  ifstream file(path.c_str());
  if(file.is_open()) {
    file >> rows >> cols;
    maze = (Cell**) malloc(sizeof(Cell*) * rows);
    rewards = (int**) malloc(sizeof(int*) * rows);
    for(int i=0;i<rows;++i) {
      maze[i] = (Cell*) malloc(sizeof(Cell) * cols);
      rewards[i] = (int*) malloc(sizeof(int) * cols);
      string types;
      file >> types;
      for(int j=0;j<cols;++j) {
        maze[i][j].row = i;
        maze[i][j].col = j;
        maze[i][j].type = types[j];
        rewards[i][j] = 0;
      }
    }
    while(!file.eof()) {
      int r, c, v;
      file >> r >> c >> v;
      rewards[r][c]= v;
    }
  }
  file.close();
}

void MazeEnv::freeMaze() {
  if(maze != NULL) {
    for(int i=0;i<rows;++i) {
      free(maze[i]);
    }
    free(maze);
    maze = NULL;
  }
}

void MazeEnv::start(int row, int col) {
  currCell.row = row;
  currCell.col = col;
}

int MazeEnv::act(const Cell& cell, int action) {
  // 80% for specified action, 20% for rest of actions
  if(rand() % 100 + 1 <= 20) {
    action = actions[rand() % actions.size()];
  }
  int row = cell.row, col = cell.col;
  switch(action) {
    case NORTH: --row; break;
    case SOUTH: ++row; break;
    case WEST: --col; break;
    case EAST: ++col; break;
  }
  if(row < 0 || row >= rows || col < 0 || col >= cols || 
      maze[row][col].type == WALL || maze[cell.row][cell.col].type == GOAL) {
    row = cell.row;
    col = cell.col;
  }
  this->currCell.row = row; 
  this->currCell.col = col; 
  this->currCell.type = maze[row][col].type;
  return rewards[cell.row][cell.col]; // reward for current state
}

void MazeEnv::prtMaze() const {
  cout << "<<Map>>" << endl;
  for(int i=0;i<rows;++i) {
    for(int j=0;j<cols;++j) {
      cout << setw(2) << (char)maze[i][j].type;
    }
    cout << endl;
  }
  cout << "<<Reward>>" << endl;
  for(int i=0;i<rows;++i) {
    for(int j=0;j<cols;++j) {
      cout << setw(2) << rewards[i][j];
    }
    cout << endl;
  }
}

vector<int> MazeEnv::getActions() const {
  return this->actions;
}

Cell MazeEnv::getCell(int row, int col) const {
  return this->maze[row][col];
}

Cell MazeEnv::getCurrCell() const {
  return this->currCell;
}

int MazeEnv::getRows() const {
  return this->rows;
}

int MazeEnv::getCols() const {
  return this->cols;
}

