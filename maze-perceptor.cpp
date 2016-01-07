#include "maze-perceptor.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

MazePerceptor::MazePerceptor(string path) {
  initMaze(path);
  actions.push_back(NONE);
  actions.push_back(UP);
  actions.push_back(DOWN);
  actions.push_back(LEFT);
  actions.push_back(RIGHT);
}

MazePerceptor::~MazePerceptor() {
  freeMaze();
}

void MazePerceptor::initMaze(string path) {
  maze = NULL;
  ifstream file(path.c_str());
  if(file.is_open()) {
    file >> rows >> cols;
    maze = (Cell**) malloc(sizeof(Cell*) * rows);
    for(int i=0;i<rows;++i) {
      maze[i] = (Cell*) malloc(sizeof(Cell) * (cols + 1));
      string types;
      file >> types;
      for(int j=0;j<cols;++j) {
        maze[i][j].row = i;
        maze[i][j].col = j;
        maze[i][j].type = types[j];
        maze[i][j].reward = getReward(types[j]);
      }
    }
  }
  file.close();
}

void MazePerceptor::freeMaze() {
  if(maze != NULL) {
    for(int i=0;i<rows;++i) {
      free(maze[i]);
    }
    free(maze);
    maze = NULL;
  }
}

void MazePerceptor::prtMaze() {
  cout << "<<Map>>" << endl;
  for(int i=0;i<rows;++i) {
    for(int j=0;j<cols;++j) {
      cout << setw(2) << maze[i][j].type;
    }
    cout << endl;
  }
  cout << "<<Reward>>" << endl;
  for(int i=0;i<rows;++i) {
    for(int j=0;j<cols;++j) {
      cout << setw(2) << maze[i][j].reward;
    }
    cout << endl;
  }
}

vector<int> MazePerceptor::getActions() {
  return actions;
}

int MazePerceptor::getRows() {
  return rows;
}

int MazePerceptor::getCols() {
  return cols;
}

Cell MazePerceptor::act(const Cell& currCell, Action action) {
  int row = currCell.row, col = currCell.col;
  switch(action) {
    case UP: --row; break;
    case DOWN: ++row; break;
    case LEFT: --col; break;
    case RIGHT: ++col; break;
  }
  if(row < 0 || row >= rows || col < 0 || col >= cols || maze[row][col].type == WALL) {
    row = currCell.row;
    col = currCell.col;
  }
  Cell nextCell;
  nextCell.row = row; 
  nextCell.col = col; 
  nextCell.type = maze[row][col].type;
  nextCell.reward = maze[row][col].reward;
  return nextCell;
}

Cell MazePerceptor::act(int row, int col, int action) {
  Cell currCell(row, col, ROAD, 0);
  return act(currCell, action);
}

int MazePerceptor::getReward(char type) {
  switch(type) {
    case TRAP: return -1;
    case GOAL: return 1;
  }
  return 0;
}
