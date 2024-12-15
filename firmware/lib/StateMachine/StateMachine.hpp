#pragma once

#include "State.hpp"

class StateMachine {
public:
    StateMachine();
    void shortPress();
    void longPress();
    void charging();
    void charged();
    void not_plugged();
    void error();
    State getState() const { return currentState; }

private:
    State currentState;
};
