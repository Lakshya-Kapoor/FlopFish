#include <chrono>
#include <iostream>

#include "player.hpp"

using namespace std;

FlopFishv4::FlopFishv4(Config config) : FlopFishv1(config), tt(config.TTSize) {}

int FlopFishv4::negamaxAlphaBeta(Position& pos, int depth, int alpha,
                                 int beta) {
    result.nodesVisited++;

    if (depth == 0) return evaluate(pos);

    vector<Move> moves = pos.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, pos);

    if (moves.empty()) {
        PositionState state = pos.getPositionState();
        if (state == PositionState::CHECKMATE) return -100000;
        if (state == PositionState::STALEMATE || state == PositionState::DRAW)
            return 0;
    }

    TTEntry* entry = tt.probe(pos.getZobristHash());
    Move ttMove;

    if (entry != nullptr) {
        ttMove = entry->getBestMove();
        if (entry->getDepth() >= depth) {
            int score = entry->getScore();
            if (entry->getBound() == Bound::EXACT) {
                return entry->getScore();
            } else if (entry->getBound() == Bound::LOWER &&
                       entry->getScore() >= beta) {
                return entry->getScore();
            } else if (entry->getBound() == Bound::UPPER &&
                       entry->getScore() <= alpha) {
                return entry->getScore();
            }
        }
    }

    int originalAlpha = alpha;
    int bestScore = -INF;
    Move bestMove;

    if (config.reorderTTMove && entry != nullptr) {
        int idx = 0;
        while (idx < moves.size() && moves[idx] != ttMove) idx++;
        if (idx < moves.size()) {
            while (idx > 0) {
                swap(moves[idx], moves[idx - 1]);
                idx--;
            }
        }
    }

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha);
        pos.undoMove(move, savedState);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        alpha = max(alpha, score);
        if (alpha >= beta) break;
    }

    Bound bound;
    if (bestScore >= beta)
        bound = Bound::LOWER;
    else if (bestScore <= originalAlpha)
        bound = Bound::UPPER;
    else
        bound = Bound::EXACT;

    tt.store(pos.getZobristHash(), depth, bestScore, bound, bestMove);

    return bestScore;
}

Result FlopFishv4::getMove(Position pos) {
    auto startTime = chrono::steady_clock::now();

    Move bestMove;
    for (int depth = 1; depth <= config.depth; depth++) {
        int bestScore = -INF;
        int alpha = -INF;
        int beta = INF;

        vector<Move> moves = pos.generateLegalMoves();
        if (config.reorderMoves) moveOrdering(moves, pos);

        for (const Move& move : moves) {
            StateInfo savedState;
            pos.makeMove(move, savedState);

            int score = -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha);
            pos.undoMove(move, savedState);

            if (score > bestScore) {
                bestMove = move;
                bestScore = score;
            }

            alpha = max(alpha, score);
        }
    }

    result.move = bestMove;
    auto endTime = chrono::steady_clock::now();
    result.timeTaken = chrono::duration<double>(endTime - startTime).count();
    return result;
}