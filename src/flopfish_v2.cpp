#include <chrono>
#include <iostream>

#include "player.hpp"
using namespace std;

FlopFishv2::FlopFishv2(Config config) : FlopFishv1(config) {}

Result FlopFishv2::getMove(Position pos) {
    auto startTime = chrono::steady_clock::now();

    Move bestMove;

    for (int depth = 1; depth <= config.depth; depth++) {
        int bestScore = -INF;
        int alpha = -INF;
        int beta = INF;

        vector<Move> moves = pos.generateLegalMoves();
        if (config.reorderMoves) moveOrdering(moves, pos);

        if (depth > 1) {
            int idx = 0;

            while (idx < moves.size() && moves[idx] != bestMove) idx++;
            while (idx > 0) {
                moves[idx] = moves[idx - 1];
                idx--;
            }

            moves[0] = bestMove;
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
        }
    }

    result.move = bestMove;
    auto endTime = chrono::steady_clock::now();
    result.timeTaken = chrono::duration<double>(endTime - startTime).count();
    return result;
}
