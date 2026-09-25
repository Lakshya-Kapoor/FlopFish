#pragma once

#include "utils.hpp"

class Zobrist {
   private:
    static U64 pieceSquareKeys[12][64];
    static U64 colorToMoveKey;
    static U64 castlingRightsKeys[16];
    static U64 enPassantFileKeys[8];

   public:
    static void initZobristKeys();

    static U64 getPieceSquareKey(Piece piece, int square);
    static U64 getColorToMoveKey();
    static U64 getCastlingRightsKey(CastlingRights rights);
    static U64 getEnPassantFileKey(int enPassantSquare);
};