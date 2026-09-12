#include "types.h"

Piece operator|(Piece a, Piece b) {
    return static_cast<Piece>(static_cast<U8>(a) | static_cast<U8>(b));
}

U8 operator&(Piece a, Piece b) {
    return static_cast<U8>(a) & static_cast<U8>(b);
}

CastlingRights operator|(CastlingRights a, CastlingRights b) {
    return static_cast<CastlingRights>(static_cast<U8>(a) | static_cast<U8>(b));
}

Color operator-(Color color) {
    return (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
}