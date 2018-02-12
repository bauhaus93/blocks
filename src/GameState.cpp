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

}       // namespace mc
