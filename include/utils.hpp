#pragma once

#define INF 1e9

#include <cstdint>

using U8 = uint8_t;
using U64 = uint64_t;

enum class Color : U8 { NONE, WHITE, BLACK };
Color operator-(Color color);

enum class Piece : U8 {
    EMPTY = 0,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

bool isWhitePiece(Piece piece);
bool isBlackPiece(Piece piece);
Color getPieceColor(Piece piece);
int getPieceValue(Piece piece);

enum class CastlingRights : U8 {
    NONE = 0,
    WHITE_KINGSIDE = 1,
    WHITE_QUEENSIDE = 2,
    BLACK_KINGSIDE = 4,
    BLACK_QUEENSIDE = 8
};

CastlingRights operator|(CastlingRights a, CastlingRights b);
CastlingRights& operator|=(CastlingRights& a, CastlingRights b);
CastlingRights operator&(CastlingRights a, CastlingRights b);
CastlingRights& operator&=(CastlingRights& a, CastlingRights b);
CastlingRights operator~(CastlingRights a);

bool castlingRightsContains(CastlingRights rights, CastlingRights check);
