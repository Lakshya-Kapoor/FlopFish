#pragma once

#include <vector>

#include "move.hpp"
#include "position.hpp"
#include "transposition_table.hpp"

struct Result {
    Move move;
    U64 nodesVisited = 0;
    double timeTaken;
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
    bool reorderTTMove = true;
    size_t TTSize = 1 << 20;
};

class FlopFishv1 : public Player {
   protected:
    Result result;
    Config config;

    int evaluate(Position& pos);
    int evaluateMaterial(Position& pos);
    int evaluatePieceSquareTables(Position& pos);

    int negamax(Position& pos, int depth);
    virtual int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta);

    void moveOrdering(std::vector<Move>& moves, const Position& pos);

   public:
    FlopFishv1(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv2 : public FlopFishv1 {
   public:
    FlopFishv2(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv3 : public FlopFishv1 {
   protected:
    // Principal Variation table, pv[i] represents the best move sequence from
    // ply i onwards. pv[i][0] best move at ply i, pv[i][1] best move at ply
    // i+1, and so on.
    Move pv[32][32];

    virtual int negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta,
                                 int ply, bool usePV);

   public:
    FlopFishv3(Config config);

    Result getMove(Position pos) override;
};

class FlopFishv4 : public FlopFishv1 {
   protected:
    TranspositionTable tt;
    int negamaxAlphaBeta(Position& pos, int depth, int alpha,
                         int beta) override;

   public:
    FlopFishv4(Config config);

    Result getMove(Position pos) override;
};