#include "transposition_table.hpp"

using namespace std;

TTEntry::TTEntry(U64 hash, int depth, int score, Bound bound, Move bestMove)
    : zobristHash(hash),
      depth(depth),
      score(score),
      bound(bound),
      bestMove(bestMove) {}

U64 TTEntry::getZobristHash() const { return zobristHash; }
int TTEntry::getDepth() const { return depth; }
int TTEntry::getScore() const { return score; }
Bound TTEntry::getBound() const { return bound; }
Move TTEntry::getBestMove() const { return bestMove; }

TranspositionTable::TranspositionTable(size_t size) {
    // Round up to the nearest power of 2
    size_t tableSize = 1;
    while (tableSize < size) {
        tableSize <<= 1;
    }
    table.resize(tableSize, nullptr);
}

void TranspositionTable::store(U64 hash, int depth, int score, Bound bound,
                               Move bestMove) {
    size_t index = hash & (table.size() - 1);
    if (table[index] != nullptr) delete table[index];

    table[index] = new TTEntry(hash, depth, score, bound, bestMove);
}

TTEntry* TranspositionTable::probe(U64 hash) {
    size_t index = hash & (table.size() - 1);
    TTEntry* entry = table[index];
    if (entry && entry->getZobristHash() == hash) {
        return entry;
    }
    return nullptr;
}