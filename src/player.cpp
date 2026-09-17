#include "player.hpp"

#include <algorithm>

using namespace std;

const int pawnTable[64] = {
    0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
    10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
    0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
    5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0};

const int knightTable[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
    0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
    15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
    -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
    0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50};

const int bishopTable[64] = {
    -20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
    0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
    10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
    -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
    0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20};

const int rookTable[64] = {0,  0,  0,  5,  5, 0,  0,  0, -5, 0, 0,  0,  0,
                           0,  0,  -5, -5, 0, 0,  0,  0, 0,  0, -5, -5, 0,
                           0,  0,  0,  0,  0, -5, -5, 0, 0,  0, 0,  0,  0,
                           -5, -5, 0,  0,  0, 0,  0,  0, -5, 5, 10, 10, 10,
                           10, 10, 10, 5,  0, 0,  0,  0, 0,  0, 0,  0};

const int queenTable[64] = {
    -20, -10, -10, -5, -5, -10, -10, -20, -10, 0,   0,   0,  0,  0,   0,   -10,
    -10, 0,   5,   5,  5,  5,   0,   -10, -5,  0,   5,   5,  5,  5,   0,   -5,
    0,   0,   5,   5,  5,  5,   0,   -5,  -10, 5,   5,   5,  5,  5,   5,   -10,
    -10, 0,   5,   0,  0,  5,   0,   -10, -20, -10, -10, -5, -5, -10, -10, -20};

const int kingTable[64] = {
    -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
    -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
    -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
    -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
    0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20};

FlopFishv1::FlopFishv1() {}
FlopFishv1::FlopFishv1(Config config) : config(config) {}

int FlopFishv1::evaluateMaterial(Position& pos) {
    int score = 0;

    for (int square = 0; square < 64; square++) {
        Piece piece = pos.getPieceAt(square);
        if (isWhitePiece(piece))
            score += getPieceValue(piece);
        else if (isBlackPiece(piece))
            score -= getPieceValue(piece);
    }
    return score;
}

int FlopFishv1::evaluateMobility(Position& pos) {
    int score = 0;

    vector<Move> whiteMoves;
    vector<Move> blackMoves;

    if (pos.getColorToMove() == Color::WHITE) {
        whiteMoves = pos.generatePseudoLegalMoves();
        pos.setColorToMove(Color::BLACK);
        blackMoves = pos.generatePseudoLegalMoves();
        pos.setColorToMove(Color::WHITE);
    } else {
        blackMoves = pos.generatePseudoLegalMoves();
        pos.setColorToMove(Color::WHITE);
        whiteMoves = pos.generatePseudoLegalMoves();
        pos.setColorToMove(Color::BLACK);
    }

    return whiteMoves.size() - blackMoves.size();
}

int FlopFishv1::evaluatePieceSquareTables(Position& pos) {
    int score = 0;
    for (int square = 0; square < 64; square++) {
        Piece piece = pos.getPieceAt(square);
        if (piece == Piece::WHITE_PAWN)
            score += pawnTable[square];
        else if (piece == Piece::WHITE_KNIGHT)
            score += knightTable[square];
        else if (piece == Piece::WHITE_BISHOP)
            score += bishopTable[square];
        else if (piece == Piece::WHITE_ROOK)
            score += rookTable[square];
        else if (piece == Piece::WHITE_QUEEN)
            score += queenTable[square];
        else if (piece == Piece::WHITE_KING)
            score += kingTable[square];
        else if (piece == Piece::BLACK_PAWN)
            score -= pawnTable[63 - square];
        else if (piece == Piece::BLACK_KNIGHT)
            score -= knightTable[63 - square];
        else if (piece == Piece::BLACK_BISHOP)
            score -= bishopTable[63 - square];
        else if (piece == Piece::BLACK_ROOK)
            score -= rookTable[63 - square];
        else if (piece == Piece::BLACK_QUEEN)
            score -= queenTable[63 - square];
        else if (piece == Piece::BLACK_KING)
            score -= kingTable[63 - square];
    }

    return score;
}

// returns evaluation relative to the side to move.
int FlopFishv1::evaluate(Position& pos) {
    PositionState state = pos.getPositionState();
    if (state == PositionState::CHECKMATE) return -100000;
    if (state == PositionState::STALEMATE || state == PositionState::DRAW)
        return 0;

    int score = 0;
    score += evaluateMaterial(pos);
    // score += evaluateMobility(pos);
    score += evaluatePieceSquareTables(pos);

    return (pos.getColorToMove() == Color::WHITE) ? score : -score;
}

int FlopFishv1::negamax(Position& pos, int depth) {
    if (depth == 0) return evaluate(pos);

    vector<Move> moves = pos.generateLegalMoves();
    if (moves.empty()) {
        PositionState state = pos.getPositionState();
        if (state == PositionState::CHECKMATE) return -100000;
        if (state == PositionState::STALEMATE || state == PositionState::DRAW)
            return 0;
    }

    int maxScore = -INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int eval = -negamax(pos, depth - 1);
        maxScore = max(maxScore, eval);

        pos.undoMove(move, savedState);
    }
    return maxScore;
}

int FlopFishv1::negamaxAlphaBeta(Position& pos, int depth, int alpha,
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

    int maxScore = -INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha);
        pos.undoMove(move, savedState);

        maxScore = max(maxScore, score);
        alpha = max(alpha, score);
        if (alpha >= beta) break;
    }

    return maxScore;
}

void FlopFishv1::moveOrdering(std::vector<Move>& moves, const Position& pos) {
    int n = moves.size();
    int i = 0;  // index of first non capture move
    for (int j = 0; j < n; j++) {
        if (moves[j].type == MoveType::CAPTURE ||
            moves[j].type == MoveType::CAPTURE_PROMOTION ||
            moves[j].type == MoveType::EN_PASSANT) {
            swap(moves[i], moves[j]);
            i++;
        }
    }

    if (config.reorderCaptures) {
        sort(moves.begin(), moves.begin() + i,
             [&pos](const Move& a, const Move& b) {
                 int aValue = getPieceValue(a.capturedPiece) * 10 -
                              getPieceValue(pos.getPieceAt(a.fromSquare));
                 int bValue = getPieceValue(b.capturedPiece) * 10 -
                              getPieceValue(pos.getPieceAt(b.fromSquare));
                 return aValue > bValue;
             });
    }
}

Result FlopFishv1::getMove(Position pos) {
    vector<Move> moves = pos.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, pos);

    int bestScore = -INF;
    int alpha = -INF;
    int beta = INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(pos, config.depth - 1, -beta, -alpha);

        pos.undoMove(move, savedState);

        if (score > bestScore) {
            bestScore = score;
            result.move = move;
        }

        alpha = max(alpha, score);
    }

    return result;
}

FlopFishv2::FlopFishv2() {}
FlopFishv2::FlopFishv2(Config config) : FlopFishv1(config) {}

Result FlopFishv2::getMove(Position pos) {
    int bestScore = -INF;
    Move bestMove;
    int alpha = -INF;
    int beta = INF;
    int depth = config.depth;

    // Keeping generation to the outside of the loop avoids generating moves
    // multipole times and also allows us to keep previous best moves from
    // shallower depths to the front
    vector<Move> moves = pos.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, pos);

    for (int depth = 1; depth <= config.depth; depth++) {
        if (depth > 1) {
            int idx = 0;

            while (moves[idx] != bestMove) idx++;
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
    return result;
}

FlopFishv3::FlopFishv3() {}
FlopFishv3::FlopFishv3(Config config) : FlopFishv1(config) {}

int FlopFishv3::negamaxAlphaBeta(Position& pos, int depth, int alpha, int beta,
                                 int ply, bool usePV) {
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

    if (usePV && depth > 1) {
        int idx = 0;

        while (idx < (int)moves.size() && moves[idx] != pv[1][ply - 1]) idx++;

        if (idx < (int)moves.size()) {
            while (idx > 0) {
                moves[idx] = moves[idx - 1];
                idx--;
            }

            moves[0] = pv[1][ply - 1];
        }
    }

    int maxScore = -INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int score =
            -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha, ply + 1, usePV);
        usePV = false;  // Only use PV for the first move at each depth

        pos.undoMove(move, savedState);

        if (score > maxScore) {
            maxScore = score;

            pv[ply][0] = move;
            for (int len = 1; len < depth; len++) {
                pv[ply][len] = pv[ply + 1][len - 1];
            }
        }
        alpha = max(alpha, score);
        if (alpha >= beta) break;
    }

    return maxScore;
}

Result FlopFishv3::getMove(Position pos) {
    int ply = 1;
    bool usePV = false;  // Use Principal Variation for move ordering

    // Keeping generation to the outside of the loop avoids generating moves
    // multipole times and also allows us to keep previous best moves from
    // shallower depths to the front
    vector<Move> moves = pos.generateLegalMoves();
    if (config.reorderMoves) moveOrdering(moves, pos);

    for (int depth = 1; depth <= config.depth; depth++) {
        int bestScore = -INF;
        int alpha = -INF;
        int beta = INF;

        if (depth > 1) {
            int idx = 0;

            while (idx < (int)moves.size() && moves[idx] != pv[1][ply - 1])
                idx++;

            if (idx < (int)moves.size()) {
                while (idx > 0) {
                    moves[idx] = moves[idx - 1];
                    idx--;
                }

                moves[0] = pv[1][ply - 1];
                usePV = true;
            }
        }

        for (const Move& move : moves) {
            StateInfo savedState;
            pos.makeMove(move, savedState);

            int score = -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha,
                                          ply + 1, usePV);
            usePV = false;

            pos.undoMove(move, savedState);

            if (score > bestScore) {
                bestScore = score;

                pv[ply][0] = move;
                for (int len = 1; len < depth; len++) {
                    pv[ply][len] = pv[ply + 1][len - 1];
                }
            }

            alpha = max(alpha, score);
        }
    }

    result.move = pv[ply][0];
    return result;
}
