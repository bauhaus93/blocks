/* Copyright 2018 Jakob Fischer <JakobFischer93@gmail.com> */

#include "GameState.hpp"

namespace mc {

GameState::GameState(std::vector<std::unique_ptr<GameState>>& stateStack_):
    stateStack { stateStack_ } {
}


}       // namespace mc
