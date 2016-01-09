#pragma once
#ifndef __QLEARN_AGENT_HPP
#define __QLEARN_AGENT_HPP

#include "maze-env.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

typedef string state_t;
typedef int action_t;
typedef float value_t;

typedef struct state_action_pair_ {
  state_t state;
  action_t action;

  bool operator==(const state_action_pair_ &other) const {
    return state == other.state && action == other.action;
  }
} sa_t;

namespace std {
  
  template <>
  struct hash<sa_t> {
    size_t operator()(const sa_t& k) const {
      return (hash<state_t>()(k.state)) ^ (hash<action_t>()((int)k.action) << 1);
    }
  };

}

class QLearningAgent {

  public:
    QLearningAgent(vector<action_t> actions, float gamma=1, float alpha=1);
    value_t get(sa_t state_action_pair);
    value_t get(state_t state, action_t action);
    void set(sa_t state_action_pair, value_t val);
    void set(state_t state, action_t action, value_t val);
    void update(state_t state, action_t action, state_t new_state, value_t reward);
    action_t getMaxAction(state_t state);
    action_t getActualAction(state_t state);
    void save(string path);
    void load(string path);

  private:
    unordered_map<sa_t, float> qvalues;
    vector<action_t> actions;
    float gamma;
    float alpha;

};

#endif
