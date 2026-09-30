#pragma once
#include <string>

#include "game_state.hpp"
#include "player.hpp"

class Game {
   private:
    GameState gameState;
    Player* whitePlayer;
    Player* blackPlayer;

    Player* getCurrentPlayer() const;

   public:
    Game(Player* white, Player* black);
    Game(Player* white, Player* black, const std::string& fen);

    void play();
};