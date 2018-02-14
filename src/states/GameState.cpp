/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GameState.hpp"

namespace mc {

GameState::GameState(sf::Window& window_, unsigned int delay_):
    window { window_ },
    delay { sf::milliseconds(delay_) },
    lastDelta { sf::milliseconds(1) },
    leave { false },
    result { true, State::NONE } {
}

void GameState::SetResultAndLeave(bool pop, State next) {
    result.pop = pop;
    result.next = next;
    leave = true;
}

StateResult GameState::Run() {
    sf::Clock clock;
    leave = false;

    while (!leave) {
        Tick();
        sf::sleep(delay);
        lastDelta = clock.restart();
    }
    return result;
}

std::string GetStateName(State state) {
    switch (state) {
    case State::NONE:   return "State::NONE";
    case State::EXIT:   return "State::EXIT";
    case State::WORLD:  return "State::WORLD";
    case State::PAUSE:  return "State::PAUSE";
    default:            return "Unhandled State";
    }
}

}       // namespace mc
