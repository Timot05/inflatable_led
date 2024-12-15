#include "StateMachine.hpp"
#include <Arduino.h>

StateMachine::StateMachine() : currentState(State::ON_1) {}

void StateMachine::shortPress() {
    switch (currentState) {
        case State::OFF:
            currentState = State::ON_1;
            break;
        case State::ON_1:
            currentState = State::ON_2;
            break;
        case State::ON_2:
            currentState = State::ON_3;
            break;
        case State::ON_3:
            currentState = State::ON_1;
            break;
        case State::CHARGING:
        case State::CHARGED:
            break;
    }
}

void StateMachine::longPress() {
    switch (currentState) {
        case State::CHARGING:
        case State::CHARGED:
        case State::OFF:
            break;
        case State::ON_1:
        case State::ON_2:
        case State::ON_3:
        case State::ERROR:
            currentState = State::OFF;
            break;
    }
}

void StateMachine::charging(){
    currentState = State::CHARGING;
}

void StateMachine::charged(){
    currentState = State::CHARGED;
}

void StateMachine::not_plugged(){
    switch (currentState) {
        case State::CHARGING:
        case State::CHARGED:
            currentState = State::ON_1;
        case State::ON_1:
        case State::ON_2:
        case State::ON_3:
            break;
    }
}

void StateMachine::error(){
    currentState = State::ERROR;
}