/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "logger/GlobalLogger.hpp"
#include "GameState.hpp"
#include "WorldState.hpp"
#include "PauseState.hpp"

namespace mc {


class StateStack {

 public:

    explicit    StateStack(sf::RenderWindow& window_);

    void        Push(State state);
    void        Run();

 private:

    void        Pop();

    sf::RenderWindow&   		                window;
    std::vector<std::unique_ptr<GameState>>     states;
};

}       // namespace mc
