#pragma once

#include <vector>

#include "move.hpp"
#include "position.hpp"

struct Result {
    Move move;
    U64 nodesVisited = 0;
};

class Player {
   public:
    virtual Result getMove(Position pos) = 0;
};

class Human : public Player {
   public:
    Result getMove(Position pos) override;
};

struct Config {
    int depth;
    bool reorderMoves = true;
    bool reorderCaptures = true;
};

class FlopFishv1 : public Player {
   protected:
    Config config;
    Result result;

    int evaluate(Position& pos);
    int evaluateMaterial(Position& pos);
    int evaluateMobility(Position& pos);
    int evaluatePieceSquareTables(Position& pos);

    int negamax(Position& pos, int depth);
    int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta);

    void moveOrdering(std::vector<Move>& moves, const Position& pos);

   public:
    FlopFishv1();
    FlopFishv1(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv2 : public FlopFishv1 {
   public:
    FlopFishv2();
    FlopFishv2(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv3 : public FlopFishv1 {
   protected:
    Move pv[32][32];  // Principal Variation table

    virtual int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta,
                                 int ply, bool usePV);

   public:
    FlopFishv3();
    FlopFishv3(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv4 : public FlopFishv3 {
   protected:
    int quiescenceSearch(Position& pos, int alpha, int beta);

    int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta, int ply,
                         bool usePV) override;

   public:
    FlopFishv4();
    FlopFishv4(Config config);
};