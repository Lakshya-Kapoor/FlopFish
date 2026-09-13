#include "../include/player.hpp"

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

int FlopFishv1::evaluateMaterial(Position& pos) {
    int score = 0;

    for (int square = 0; square < 64; square++) {
        Piece piece = pos.getPieceAt(square);
        if (piece == (Piece::WHITE | Piece::PAWN))
            score += 100;
        else if (piece == (Piece::WHITE | Piece::KNIGHT))
            score += 320;
        else if (piece == (Piece::WHITE | Piece::BISHOP))
            score += 330;
        else if (piece == (Piece::WHITE | Piece::ROOK))
            score += 500;
        else if (piece == (Piece::WHITE | Piece::QUEEN))
            score += 900;
        else if (piece == (Piece::BLACK | Piece::PAWN))
            score -= 100;
        else if (piece == (Piece::BLACK | Piece::KNIGHT))
            score -= 320;
        else if (piece == (Piece::BLACK | Piece::BISHOP))
            score -= 330;
        else if (piece == (Piece::BLACK | Piece::ROOK))
            score -= 500;
        else if (piece == (Piece::BLACK | Piece::QUEEN))
            score -= 900;
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
        if (piece == (Piece::WHITE | Piece::PAWN))
            score += pawnTable[square];
        else if (piece == (Piece::WHITE | Piece::KNIGHT))
            score += knightTable[square];
        else if (piece == (Piece::WHITE | Piece::BISHOP))
            score += bishopTable[square];
        else if (piece == (Piece::WHITE | Piece::ROOK))
            score += rookTable[square];
        else if (piece == (Piece::WHITE | Piece::QUEEN))
            score += queenTable[square];
        else if (piece == (Piece::BLACK | Piece::PAWN))
            score -= pawnTable[63 - square];
        else if (piece == (Piece::BLACK | Piece::KNIGHT))
            score -= knightTable[63 - square];
        else if (piece == (Piece::BLACK | Piece::BISHOP))
            score -= bishopTable[63 - square];
        else if (piece == (Piece::BLACK | Piece::ROOK))
            score -= rookTable[63 - square];
        else if (piece == (Piece::BLACK | Piece::QUEEN))
            score -= queenTable[63 - square];
    }

    return score;
}

int FlopFishv1::evaluate(Position& pos) {
    PositionState state = pos.getPositionState();
    if (state == PositionState::CHECKMATE) return -100000;
    if (state == PositionState::STALEMATE || state == PositionState::DRAW)
        return 0;

    int score = 0;
    score += evaluateMaterial(pos);
    score += evaluateMobility(pos);
    score += evaluatePieceSquareTables(pos);
    return score;
}

int FlopFishv1::negamax(Position& pos, int depth) {
    if (depth == 0) {
        if (pos.getColorToMove() == Color::WHITE)
            return evaluate(pos);
        else
            return -evaluate(pos);
    }

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
    if (depth == 0) {
        if (pos.getColorToMove() == Color::WHITE)
            return evaluate(pos);
        else
            return -evaluate(pos);
    }

    vector<Move> moves = pos.generateLegalMoves();
    if (moves.empty()) {
        PositionState state = pos.getPositionState();
        if (state == PositionState::CHECKMATE) return -100000;
        if (state == PositionState::STALEMATE || state == PositionState::DRAW)
            return 0;
    }

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int eval = -negamaxAlphaBeta(pos, depth - 1, -beta, -alpha);
        pos.undoMove(move, savedState);

        if (eval >= beta) return beta;  // Beta cutoff
        alpha = max(alpha, eval);
    }

    return alpha;
}

Move FlopFishv1::getMove(Position pos) {
    int depth = 5;

    vector<Move> moves = pos.generateLegalMoves();
    Move bestMove;
    int maxScore = -INF;

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        int score = -negamaxAlphaBeta(pos, depth - 1, -INF, -maxScore);

        if (score > maxScore) {
            maxScore = score;
            bestMove = move;
        }

        pos.undoMove(move, savedState);
    }

    return bestMove;
}