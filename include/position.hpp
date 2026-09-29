#pragma once

#include <string>
#include <vector>

#include "move.hpp"
#include "utils.hpp"

const int knightDir[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                             {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

const int bishopDir[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

const int rookDir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

const int kingDir[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

struct Pin {
    bool isPinned = false;
    int dr;
    int dc;
};

class LegalityInfo {
   private:
    int kingSquare;
    bool inCheck;
    Pin pinnedSquare[64];
    bool evasionSquares[64];

   public:
    LegalityInfo(int kingSquare, bool inCheck, Pin pinnedSquare[],
                 bool evasionSquares[]);

    bool legalityRespected(int fromSquare, int toSquare) const;

    int getKingSquare() const;
    bool isInCheck() const;
    bool isPinned(int square) const;
};

struct StateInfo {
    CastlingRights castlingRights;
    int enPassantSquare;
    int halfmoveClock;
    int fullmoveNumber;
    U64 zobristHash;
};

enum class PositionState { ONGOING, CHECKMATE, STALEMATE, DRAW };

class Position {
   private:
    Piece board[64];
    Color colorToMove;
    CastlingRights castlingRights;
    int enPassantSquare;
    int halfmoveClock;
    int fullmoveNumber;
    U64 zobristHash;

    void initPos();
    void parseFENPos(const std::string& fen);

    void generateKnightMoves(int square, std::vector<Move>& moves,
                             const LegalityInfo& info);
    void generateBishopMoves(int square, std::vector<Move>& moves,
                             const LegalityInfo& info);
    void generateRookMoves(int square, std::vector<Move>& moves,
                           const LegalityInfo& info);
    void generateQueenMoves(int square, std::vector<Move>& moves,
                            const LegalityInfo& info);
    void generateKingMoves(int square, std::vector<Move>& moves);
    void generatePawnMoves(int square, std::vector<Move>& moves,
                           const LegalityInfo& info);
    void generateCastlingMoves(std::vector<Move>& moves);
    void generateEnPassantMoves(std::vector<Move>& moves,
                                const LegalityInfo& info);

    void removeCastlingRights(const Move& move);

    bool isInsideBoard(int r, int c) const;

   public:
    Position();
    Position(const std::string& fen);

    Piece getPieceAt(int square) const;
    Color getColorToMove() const;
    CastlingRights getCastlingRights() const;
    int getEnPassantSquare() const;
    int getHalfmoveClock() const;
    int getFullmoveNumber() const;
    PositionState getPositionState();

    U64 getZobristHash() const;
    U64 generateZobristHash();

    void print() const;
    std::string toFEN() const;

    bool isSquareAttacked(int square, Color byColor) const;
    bool inCheck(Color color) const;

    void makeMove(const Move& move);
    void makeMove(const Move& move, StateInfo& saveState);
    void undoMove(const Move& move, const StateInfo& savedState);

    std::vector<Move> generateLegalMoves();
};