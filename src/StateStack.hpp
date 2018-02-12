/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <memory>

#include "logger/GlobalLogger.hpp"
#include "GameState.hpp"
#include "WorldState.hpp"
#include "PauseState.hpp"

namespace mc {


class StateStack {

 public:

                StateStack(sf::Window& window_);

    void        Push(State state);
    void        Run();

 private:

    void        Pop();


    sf::Window&   		                        window;
    std::vector<std::unique_ptr<GameState>>     states;
};

}       // namespace mc

