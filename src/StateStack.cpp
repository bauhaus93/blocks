/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "StateStack.hpp"

namespace mc {

StateStack::StateStack(sf::Window& window_):
    window { window_ } {
}

void StateStack::Push(State state) {
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
    states.pop_back();
}


void StateStack::Run() {
    while(!states.empty()) {
        StateResult result = states.back()->Run();
        DEBUG("Changing state");
        if (result.next == State::EXIT) {
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



}       // namespace mc
