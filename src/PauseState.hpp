/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "logger/GlobalLogger.hpp"
#include "GameState.hpp"

namespace mc {

class PauseState: public GameState {

 public: 

                        PauseState(std::vector<std::unique_ptr<GameState>>& stateStack_, sf::Window& window_);
                        PauseState() = default;
    void                Run() override;

 private:

    void                HandleEvents();
    
    sf::Window&         window;
    bool                leave;
};
 

}       // namespace mc
