#pragma once

#include <vector>

#include "move.hpp"
#include "utils.hpp"

enum class Bound : U8 { EXACT, LOWER, UPPER };

class TTEntry {
   private:
    U64 zobristHash;
    int depth;
    int score;
    Bound bound;
    Move bestMove;
    bool valid;

   public:
    TTEntry();
    TTEntry(U64 hash, int depth, int score, Bound bound, Move bestMove);

    U64 getZobristHash() const;
    int getDepth() const;
    int getScore() const;
    Bound getBound() const;
    Move getBestMove() const;
    bool isValid() const;
};

class TranspositionTable {
   private:
    std::vector<TTEntry> table;

   public:
    // size represents the number of entries in the transposition table, it'll
    // be rounded up to the nearest power of 2
    TranspositionTable(size_t size);
    void store(U64 hash, int depth, int score, Bound bound, Move bestMove);
    TTEntry probe(U64 hash) const;
};