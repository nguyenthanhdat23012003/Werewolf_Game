#ifndef GAMESTATE_H
#define GAMESTATE_H

#pragma once

class GameState {
public:
    enum class State {
        LOBBY_LIST,
        IN_LOBBY,
        IN_GAME
    };

    // Static getter method to get the current state
    static State getCurrentState();

    // Static setter method to set the current state
    static void setCurrentState(State newState);

private:
    static State currentState;
};

#endif // GAMESTATE_H