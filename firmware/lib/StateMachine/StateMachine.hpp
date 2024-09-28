#pragma once

#include "State.hpp"

class StateMachine {
public:
    StateMachine();
    void update();
    void shortPress();
    void longPress();
    State getState() const { return currentState; }

private:
    State currentState;
};
