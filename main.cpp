#include "maze-perceptor.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
  srand(time(NULL));

  MazePerceptor mp("map/map1.txt");
  vector<Action> actions = mp.getActions();
  for(int i=0;i<mp.getRows();i++) {
    for(int j=0;j<mp.getCols();j++) {
      Cell cell = mp.act(i, j, actions[rand() % actions.size()]);
      cout << cell << endl;
    }
  }
  return 0;
}
