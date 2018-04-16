/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "PauseState.hpp"

namespace mc {

PauseState::PauseState(sf::Window& window_):
    GameState(window_, 100) {
}

State PauseState::GetState() const {
    return State::PAUSE;
}

void PauseState::Tick() {
    HandleEvents();
}

void PauseState::HandleEvents() {
    sf::Event event;

    while (window.pollEvent(event)) {
        switch(event.type) {
        case sf::Event::Closed:
            SetResultAndLeave(true, State::EXIT);
            return;
        case sf::Event::GainedFocus:
            TRACE("Gained FOCUS!");
            SetResultAndLeave(true, State::NONE);
            return;
        default:
            break;
        }
    }
}


}       // namespace mc
