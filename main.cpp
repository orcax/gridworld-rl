#include "maze-env.hpp"
#include "qlearn-agent.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iomanip>

int main(int argc, char** argv) {
  srand(time(NULL));

  MazeEnv me("map/map1.txt");
  QLearningAgent agent(me.getActions(), 0.9, 0.9);
  Cell state1, state2;
  action_t action;
  value_t reward;

  me.valueIterate();

  for(int i=0;i<1000000;i++) {
    me.start(0, 0);
    do {
      state1 = me.getCurrCell();
      action = agent.getActualAction(state1.str());
      reward = me.act(state1, action);
      state2 = me.getCurrCell();
      agent.update(state1.str(), action, state2.str(), reward);
      value_t qval = agent.get(state1.str(), action);
      //cout << state1 << " " << action << " " << reward << " " << qval << endl;
    } while(state2.type != GOAL);
  }

  cout << "Q Values:" << endl;
  for(int r=0;r<me.getRows();++r) {
    for(int c=0;c<me.getCols();++c) {
      Cell state = me.getCell(r, c);
      action_t action = agent.getMaxAction(state.str());
      printf("%.2f ", agent.get(state.str(), action));
    }
    cout << endl;
  }

  cout << "Best Actions:" << endl;
  for(int r=0;r<me.getRows();++r) {
    for(int c=0;c<me.getCols();++c) {
      Cell state = me.getCell(r, c);
      if(state.type == WALL) {
        cout << "# ";
        continue;
      }
      if(state.type == GOAL) {
        cout << "T ";
        continue;
      }
      action_t action = agent.getMaxAction(state.str());
      switch(action) {
        case NONE: cout << "x "; break;
        case NORTH: cout << "↑ "; break;
        case SOUTH: cout << "↓ "; break;
        case WEST: cout << "← "; break;
        case EAST: cout << "→ "; break;
      }
    }
    cout << endl;
  }

  return 0;
}
