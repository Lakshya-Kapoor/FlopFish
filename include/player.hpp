#pragma once

#include <vector>

#include "move.hpp"
#include "position.hpp"

class Player {
   public:
    virtual Move getMove(Position pos) = 0;
};

class Human : public Player {
   public:
    Move getMove(Position pos) override;
};

class FlopFishv1 : public Player {
   private:
    int evaluate(Position& pos);
    int evaluateMaterial(Position& pos);
    int evaluateMobility(Position& pos);
    int evaluatePieceSquareTables(Position& pos);

    int negamax(Position& pos, int depth);
    int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta);

   public:
    FlopFishv1();

    Move getMove(Position pos) override;
};