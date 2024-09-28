#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine() : currentState(State::OFF) {}

void StateMachine::update() {
    switch (currentState) {
        case State::OFF:
            break;
        case State::ON:
            currentState = State::INCREMENT;
            break;
        case State::INCREMENT:
            currentState = State::ON;
            break;
    }
}

void StateMachine::shortPress() {
    switch (currentState) {
        case State::OFF:
            currentState = State::ON;
            break;
        case State::ON:
            currentState = State::OFF;
            break;
        case State::INCREMENT:
            currentState = State::OFF;
            break;
    }
}

void StateMachine::longPress() {
    switch (currentState) {
        case State::OFF:
            break;
        case State::ON:
            break;
        case State::INCREMENT:
            currentState = State::ON;
            break;
    }
}