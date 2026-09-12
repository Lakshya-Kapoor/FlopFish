#pragma once

#include <cstdint>

using U8 = uint8_t;

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
U8 operator&(Piece a, Piece b);

enum class CastlingRights : U8 {
    WHITE_KINGSIDE = 1,
    WHITE_QUEENSIDE = 2,
    BLACK_KINGSIDE = 4,
    BLACK_QUEENSIDE = 8
};

CastlingRights operator|(CastlingRights a, CastlingRights b);

enum class Color { WHITE = 1, BLACK = -1 };

Color operator-(Color color);