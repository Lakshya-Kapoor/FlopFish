/*
    FlopFish v1: A simple chess engine that uses the negamax algorithm with
   alpha-beta pruning to search for the best move. Move reordering is
   implemented to improve the pruning.
*/

#include <algorithm>
#include <chrono>

#include "player.hpp"

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
            moves[j].type == MoveType::EN_PASSANT_CAPTURE) {
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
    auto startTime = chrono::steady_clock::now();

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

    auto endTime = chrono::steady_clock::now();
    result.timeTaken = chrono::duration<double>(endTime - startTime).count();

    return result;
}
