#ifndef STATE_CONTROLLER_H
#define STATE_CONTROLLER_H

#include <map>
#include "../state_config/state_config.h"

/**
 * @brief 状態を管理し、また現在の状態に応じて対応する振舞いを実行するクラス
 *
 */
class StateController {
  StateInterface* state;

 public:
  StateController(StateInterface* state) : state(nullptr) {
    this->state = state;
  }

  StateList ChangeState(StateList current_state, int event_kind,
                        StateTransitionTable state_transition_table, StateInterface* states[]) {
    this->state->PostChangeState();
    std::pair<StateList, int> next_state = std::make_pair(current_state, event_kind);
    current_state = state_transition_table[next_state];
    this->state = states[current_state];
    this->state->PreChangeState();
    return current_state;
  }

  int WasBtnAPressed() {
    return state->WasBtnAPressed();
  }
  int WasBtnBPressed() {
    return state->WasBtnBPressed();
  }
  int WasBtnCPressed() {
    return state->WasBtnCPressed();
  }

  int SpecialEvent(EventKind kind) {
    return state->SpecialEvent(kind);
  }
};

#endif