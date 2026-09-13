#pragma once

#define INF 1e9

#include <cstdint>

using U8 = uint8_t;
using U64 = uint64_t;

enum class Color : U8 { NONE, WHITE, BLACK };
Color operator-(Color color);

enum class Piece : U8 {
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6,

    WHITE = 8,
    BLACK = 16
};

Piece operator|(Piece a, Piece b);
Piece operator&(Piece a, Piece b);

bool isWhitePiece(Piece piece);
bool isBlackPiece(Piece piece);
Color getPieceColor(Piece piece);

enum class CastlingRights : U8 {
    NONE = 0,
    WHITE_KINGSIDE = 1,
    WHITE_QUEENSIDE = 2,
    BLACK_KINGSIDE = 4,
    BLACK_QUEENSIDE = 8
};

CastlingRights operator|(CastlingRights a, CastlingRights b);
CastlingRights& operator&=(CastlingRights& a, CastlingRights b);
CastlingRights operator&(CastlingRights a, CastlingRights b);
CastlingRights& operator&=(CastlingRights& a, CastlingRights b);
CastlingRights operator~(CastlingRights a);

bool castlingRightsContains(CastlingRights rights, CastlingRights check);