#pragma once

#include "utils.hpp"

// Singelton class that holds the Zobrist keys for the chess engine.
class ZobristKeys {
   public:
    static ZobristKeys* getKeys();

    U64 getPieceSquareKey(Piece piece, int square);
    U64 getColorToMoveKey();
    U64 getCastlingRightsKey(CastlingRights rights);
    U64 getEnPassantKey(int enPassantSquare);

   private:
    inline static ZobristKeys* _keys = nullptr;

    U64 pieceSquareKeys[12][64];
    U64 colorToMoveKey;
    U64 castlingRightsKeys[16];
    U64 enPassantFileKeys[8];

    ZobristKeys();
};