#include <iostream>

#include "player.hpp"

using namespace std;

FlopFishv5::FlopFishv5(Config config) : FlopFishv4(config) {}

int FlopFishv5::quiescenceSearch(GameState& gameState, int alpha, int beta) {
    result.nodesVisited++;

    vector<Move> legalMoves = gameState.generateLegalMoves();

    PositionState state = gameState.getPositionState(legalMoves);
    if (state == PositionState::CHECKMATE) return MATE_SCORE;
    if (state != PositionState::ONGOING) return 0;

    TTEntry entry = tt.probe(gameState.getZobristHash());
    Move ttMove;

    if (entry.isValid()) {
        int score = entry.getScore();
        if (entry.getBound() == Bound::EXACT) {
            return entry.getScore();
        } else if (entry.getBound() == Bound::LOWER &&
                   entry.getScore() >= beta) {
            return entry.getScore();
        } else if (entry.getBound() == Bound::UPPER &&
                   entry.getScore() <= alpha) {
            return entry.getScore();
        }
        ttMove = entry.getBestMove();
    }

    vector<Move> moves;
    int bestScore = -INF;
    int originalAlpha = alpha;
    Move bestMove;

    // not in check so we can do a stand pat evaluation and only consider
    // tactical moves like captures and promotions
    if (!gameState.inCheck(gameState.getColorToMove())) {
        int standPat = evaluate(gameState);
        bestScore = standPat;

        if (standPat >= beta) return standPat;
        alpha = max(alpha, standPat);

        for (const Move& move : legalMoves) {
            if (move.type == MoveType::CAPTURE ||
                move.type == MoveType::EN_PASSANT_CAPTURE ||
                move.type == MoveType::CAPTURE_PROMOTION ||
                move.type == MoveType::QUIET_PROMOTION) {
                moves.push_back(move);
            }
        }
    }
    // In check so we need to consider all moves to get out of check, standPat
    // evaluation is not valid
    else {
        moves = legalMoves;
    }

    // reoder moves to improve alpha beta pruning efficiency
    if (config.reorderMoves) moveOrdering(moves, gameState);
    if (config.reorderTTMove && entry.isValid()) {
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
        gameState.makeMove(move, savedState);

        int score = -quiescenceSearch(gameState, -beta, -alpha);
        gameState.undoMove(move, savedState);

        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }

        alpha = max(score, alpha);
        if (alpha >= beta) break;
    }

    Bound bound;
    if (bestScore >= beta)
        bound = Bound::LOWER;
    else if (bestScore <= originalAlpha)
        bound = Bound::UPPER;
    else
        bound = Bound::EXACT;

    tt.store(gameState.getZobristHash(), 0, bestScore, bound, bestMove);

    return bestScore;
}

int FlopFishv5::negamaxAlphaBeta(GameState& gameState, int depth, int alpha,
                                 int beta) {
    if (depth == 0) return quiescenceSearch(gameState, alpha, beta);

    result.nodesVisited++;

    vector<Move> moves = gameState.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, gameState);

    PositionState state = gameState.getPositionState(moves);
    if (state == PositionState::CHECKMATE) return MATE_SCORE;
    if (state != PositionState::ONGOING) return 0;

    TTEntry entry = tt.probe(gameState.getZobristHash());

    if (entry.isValid()) {
        if (entry.getDepth() >= depth) {
            int score = entry.getScore();
            if (entry.getBound() == Bound::EXACT) {
                return entry.getScore();
            } else if (entry.getBound() == Bound::LOWER &&
                       entry.getScore() >= beta) {
                return entry.getScore();
            } else if (entry.getBound() == Bound::UPPER &&
                       entry.getScore() <= alpha) {
                return entry.getScore();
            }
        }
    }

    int originalAlpha = alpha;
    int bestScore = -INF;
    Move bestMove;

    if (config.reorderTTMove && entry.isValid()) {
        Move ttMove = entry.getBestMove();

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
        gameState.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(gameState, depth - 1, -beta, -alpha);
        gameState.undoMove(move, savedState);

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

    tt.store(gameState.getZobristHash(), depth, bestScore, bound, bestMove);

    return bestScore;
}
