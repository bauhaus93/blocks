/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "GameState.hpp"

namespace mc {

class PauseState: public GameState {

 public: 

                        PauseState(sf::Window& window_);
                        PauseState() = default;
    void                Tick();

 private:
    void                HandleEvents();
};
 

}       // namespace mc
