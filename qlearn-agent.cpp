#include "qlearn-agent.hpp"
#include <fstream>

using namespace std;

QLearningAgent::QLearningAgent(vector<action_t> actions, float gamma, float alpha) {
  this->actions = actions;
  this->gamma = gamma;
  this->alpha = alpha;
}

value_t QLearningAgent::get(sa_t state_action_pair) {
  return this->qvalues[state_action_pair];
}

value_t QLearningAgent::get(state_t state, action_t action) {
  return get({state, action});
}

void QLearningAgent::set(sa_t state_action_pair, value_t val) {
  this->qvalues[state_action_pair] = val;
}

void QLearningAgent::set(state_t state, action_t action, value_t val) {
  sa_t key = {state, action};
  set(key, val);
}

void QLearningAgent::update(state_t state, action_t action, state_t new_state, value_t reward) {
  value_t max_qval = get(new_state, getMaxAction(new_state));
  value_t new_qval = get(state, action) + alpha * (reward + gamma * max_qval - get(state, action));
  set(state, action, new_qval);
}

action_t QLearningAgent::getMaxAction(state_t state) {
  vector<action_t> max_actions = {actions[0]};
  value_t max_q = get(state, actions[0]);
  for(int i=1;i<actions.size();++i) {
    value_t q = get(state, actions[i]);
    if(q < max_q) continue;
    if(q > max_q) {
      max_q = q;
      max_actions.clear();
    }
    max_actions.push_back(actions[i]);
  }
  return max_actions[rand() % max_actions.size()];
}

action_t QLearningAgent::getActualAction(state_t state) {
  int percent = rand() % 100 + 1;
  if(percent <= 80) return getMaxAction(state);
  return actions[rand() % actions.size()];
}

void QLearningAgent::save(string path) {
  ofstream file(path);
  for(auto it=qvalues.begin();it!=qvalues.end();++it) {
    sa_t sa_pair = it->first;
    file << sa_pair.state << " " << sa_pair.action << " " << it->second << endl;
  }
  file.close();
}

void QLearningAgent::load(string path) {
  ifstream file(path);
  state_t state;
  action_t action;
  value_t qval;
  qvalues.clear();
  while(file >> state >> action >> qval) {
    set(state, action, qval);
  }
  file.close();
}

