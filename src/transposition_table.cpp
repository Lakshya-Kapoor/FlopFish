#include "transposition_table.hpp"

using namespace std;

TTEntry::TTEntry() : valid(false) {}

TTEntry::TTEntry(U64 hash, int depth, int score, Bound bound, Move bestMove)
    : zobristHash(hash),
      depth(depth),
      score(score),
      bound(bound),
      bestMove(bestMove),
      valid(true) {}

U64 TTEntry::getZobristHash() const { return zobristHash; }
int TTEntry::getDepth() const { return depth; }
int TTEntry::getScore() const { return score; }
Bound TTEntry::getBound() const { return bound; }
Move TTEntry::getBestMove() const { return bestMove; }
bool TTEntry::isValid() const { return valid; }

TranspositionTable::TranspositionTable(size_t size) {
    // Round up to the nearest power of 2
    size_t tableSize = 1;
    while (tableSize < size) {
        tableSize <<= 1;
    }
    table.resize(tableSize);
}

void TranspositionTable::store(U64 hash, int depth, int score, Bound bound,
                               Move bestMove) {
    size_t index = hash & (table.size() - 1);
    TTEntry entry = table[index];

    if (!entry.isValid() || depth >= entry.getDepth()) {
        table[index] = TTEntry(hash, depth, score, bound, bestMove);
    }
}

TTEntry TranspositionTable::probe(U64 hash) const {
    size_t index = hash & (table.size() - 1);
    TTEntry entry = table[index];

    if (entry.isValid() && entry.getZobristHash() == hash) return entry;

    return TTEntry();  // Return an invalid entry if not found
}