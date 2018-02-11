/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

#include <SFML/Window.hpp>
#include <SFML/System.hpp>

namespace mc {

enum class State {
    NONE,
    EXIT,
    WORLD,
    PAUSE
};

struct StateResult {
    bool pop;
    State next;
};

class GameState {

 public: 

                        GameState(sf::Window& window_, unsigned int delay_);
                        GameState(const GameState& other) = delete;
    virtual             ~GameState() = default;
    StateResult         Run();

 protected:
    virtual void        Tick() = 0;
    void                SetResultAndLeave(bool pop, State next);

    sf::Window&         window;
    sf::Time            delay;
    sf::Time            lastDelta;
    bool                leave;
    StateResult         result;
};






}       // namespace mc