#include "GameState.h"

GameState::State GameState::currentState = GameState::State::LOBBY_LIST;

GameState::State GameState::getCurrentState() {
    return currentState;
}

// Static setter method to set the current state
void GameState::setCurrentState(State newState) {
    currentState = newState;
}