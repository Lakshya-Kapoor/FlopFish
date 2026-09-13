#pragma once
#include <string>

#include "player.hpp"
#include "position.hpp"

class Game {
   private:
    Position pos;
    Player* whitePlayer;
    Player* blackPlayer;

    Player* getCurrentPlayer() const;

   public:
    Game(Player* white, Player* black);
    Game(Player* white, Player* black, const std::string& fen);

    void play();
};