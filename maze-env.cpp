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
  values = NULL;
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
  if(rewards != NULL) {
    for(int i=0;i<rows;++i) {
      free(rewards[i]);
    }
    free(rewards);
    rewards = NULL;
  }
  if(values != NULL) {
    for(int i=0;i<rows;++i) {
      free(values[i]);
    }
    free(values);
    values = NULL;
  }
}

void MazeEnv::start(int row, int col) {
  currCell.row = row;
  currCell.col = col;
}

int MazeEnv::act(const Cell& cell, int action) {
  // 80% for specified action, 20% for rest of actions
  //if(rand() % 100 + 1 <= 20) {
  //  action = actions[rand() % actions.size()];
  //}
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
  return rewards[row][col]; // reward for current state
}

Cell MazeEnv::nextCell(const Cell& cell, int action) {
  //if(rand() % 100 + 1 <= 20) {
  //  action = actions[rand() % actions.size()];
  //}
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
  Cell newCell;
  newCell.row = row;
  newCell.col = col;
  newCell.type = maze[row][col].type; 
  return newCell;
}

void MazeEnv::valueIterate(float gamma) {
  values = (double**) malloc(sizeof(double*) * rows);
  for(int r=0;r<rows;++r) {
    values[r] = (double*) malloc(sizeof(double) * cols);
    for(int c=0;c<cols;++c) {
      values[r][c] = 0.0;
    }
  }
  for(int i=0;i<10000;++i) {
    for(int r=0;r<rows;++r) {
      for(int c=0;c<cols;++c) {
        double maxv = -9999;
        for(int a=0;a<actions.size();++a) {
          Cell cell = nextCell(maze[r][c], actions[a]);
          double tmp = rewards[cell.row][cell.col] + gamma * values[cell.row][cell.col];
          maxv = tmp > maxv ? tmp : maxv;
        }
        values[r][c] = maxv;
      }
    }
  }
  //double maxv = -9999;
  //for(int r=0;r<rows;++r) {
  //  for(int c=0;c<cols;++c) {
  //    if(values[r][c] > maxv) maxv = values[r][c];
  //  }
  //}
  //for(int r=0;r<rows;++r) {
  //  for(int c=0;c<cols;++c) {
  //    values[r][c] /= maxv;
  //  }
  //}
  cout << "Value Iteration:" << endl;
  for(int r=0;r<rows;++r) {
    for(int c=0;c<cols;++c) {
      printf("%.2f ", values[r][c]);
    }
    cout << endl;
  }
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

