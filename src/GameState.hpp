/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#pragma once

#include <vector>
#include <memory>

namespace mc {

class GameState {

 public: 

                        GameState(std::vector<std::unique_ptr<GameState>>& stateStack_);
                        GameState(const GameState& other) = delete;
    virtual             ~GameState() = default;
    virtual void        Run() = 0;

 protected:
    std::vector<std::unique_ptr<GameState>>&     stateStack;
};






}       // namespace mc
