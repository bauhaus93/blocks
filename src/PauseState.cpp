/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "PauseState.hpp"

namespace mc {

PauseState::PauseState(std::vector<std::unique_ptr<GameState>>& stateStack_, sf::Window& window_):
    GameState(stateStack_),
    window { window_ },
    leave { false } {
}

void PauseState::Run() {
    DEBUG("Entering PauseState");

    while(!leave) {
        HandleEvents();
        sf::sleep(sf::milliseconds(100));
    }

    DEBUG("Leaving PauseState");
}

void PauseState::HandleEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            stateStack.clear();
            leave = true;
            return;
        case sf::Event::GainedFocus:
            TRACE("Gained FOCUS!");
            stateStack.pop_back();
            leave = true;
            return;
        default: 
            break;
        }
    }
}


}       // namespace mc
