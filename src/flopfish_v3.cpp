#include <chrono>

#include "player.hpp"
using namespace std;

FlopFishv3::FlopFishv3(Config config) : FlopFishv1(config) {}

int FlopFishv3::negamaxAlphaBeta(GameState& gameState, int depth, int alpha,
                                 int beta, int ply, bool usePV) {
    result.nodesVisited++;
    if (depth == 0) return evaluate(gameState);

    vector<Move> moves = gameState.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, gameState);

    PositionState state = gameState.getPositionState(moves);
    if (state == PositionState::CHECKMATE) return MATE_SCORE;
    if (state != PositionState::ONGOING) return 0;

    if (usePV && depth > 1) {
        int idx = 0;

        while (idx < moves.size() && moves[idx] != pv[1][ply - 1]) idx++;

        while (idx > 0) {
            moves[idx] = moves[idx - 1];
            idx--;
        }

        moves[0] = pv[1][ply - 1];
    }

    int bestScore = -INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        gameState.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(gameState, depth - 1, -beta, -alpha,
                                      ply + 1, usePV);
        usePV = false;  // Only use PV for the first move at each depth

        gameState.undoMove(move, savedState);

        if (score > bestScore) {
            bestScore = score;
        }

        if (score > alpha) {
            alpha = score;

            pv[ply][0] = move;
            for (int len = 1; len < depth; len++) {
                pv[ply][len] = pv[ply + 1][len - 1];
            }
        }

        if (alpha >= beta) break;
    }

    return bestScore;
}

Result FlopFishv3::getMove(GameState gameState) {
    auto startTime = chrono::steady_clock::now();

    int ply = 1;
    bool usePV = false;  // Use Principal Variation for move ordering

    for (int depth = 1; depth <= config.depth; depth++) {
        int bestScore = -INF;
        int alpha = -INF;
        int beta = INF;

        vector<Move> moves = gameState.generateLegalMoves();
        if (config.reorderMoves) moveOrdering(moves, gameState);

        if (depth > 1) {
            int idx = 0;

            while (idx < moves.size() && moves[idx] != pv[1][ply - 1]) idx++;

            while (idx > 0) {
                moves[idx] = moves[idx - 1];
                idx--;
            }

            moves[0] = pv[1][ply - 1];
            usePV = true;
        }

        for (const Move& move : moves) {
            StateInfo savedState;
            gameState.makeMove(move, savedState);

            int score = -negamaxAlphaBeta(gameState, depth - 1, -beta, -alpha,
                                          ply + 1, usePV);
            usePV = false;

            gameState.undoMove(move, savedState);

            if (score > bestScore) {
                bestScore = score;
            }

            if (score > alpha) {
                alpha = score;

                pv[ply][0] = move;
                for (int len = 1; len < depth; len++) {
                    pv[ply][len] = pv[ply + 1][len - 1];
                }
            }
        }
    }

    result.move = pv[ply][0];
    auto endTime = chrono::steady_clock::now();
    result.timeTaken = chrono::duration<double>(endTime - startTime).count();
    return result;
}