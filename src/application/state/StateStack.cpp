/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "StateStack.hpp"

namespace blocks {

StateStack::StateStack(sf::Window& window_):
    window { window_ } {
}

void StateStack::Push(State state) {
    TRACE("Pushing new state on stack: ", GetStateName(state));
    switch(state) {
    case State::WORLD:
        states.emplace_back(std::make_unique<WorldState>(window));
        break;
    case State::PAUSE:
        states.emplace_back(std::make_unique<PauseState>(window));
        break;
    default:
        break;
    }
}

void StateStack::Pop() {
    TRACE("Popping state from stack: ", GetStateName(states.back()->GetState()));
    states.pop_back();
}


void StateStack::Run() {
    while(!states.empty()) {
        INFO("Entering ", GetStateName(states.back()->GetState()));
        StateResult result = states.back()->Run();

        if (result.next == State::EXIT) {
            TRACE("Clearing state stack");
            states.clear();
        } else {
            if (result.pop) {
                Pop();
            }
            if (result.next != State::NONE) {
                Push(result.next);
            }
        }
    }
}



}       // namespace blocks
