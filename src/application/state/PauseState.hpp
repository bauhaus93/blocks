/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "logger/GlobalLogger.hpp"
#include "GameState.hpp"

namespace blocks {

class PauseState: public GameState {

 public:

    explicit            PauseState(sf::Window& window_);
                        PauseState() = default;
    void                Tick() override;
    State               GetState() const override;

 private:
    void                HandleEvents();
};


}       // namespace blocks
