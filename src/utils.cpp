#include "../include/utils.hpp"

Piece operator|(Piece a, Piece b) {
    return static_cast<Piece>(static_cast<U8>(a) | static_cast<U8>(b));
}

Piece operator&(Piece a, Piece b) {
    return static_cast<Piece>(static_cast<U8>(a) & static_cast<U8>(b));
}

CastlingRights operator|(CastlingRights a, CastlingRights b) {
    return static_cast<CastlingRights>(static_cast<U8>(a) | static_cast<U8>(b));
}

CastlingRights& operator|=(CastlingRights& a, CastlingRights b) {
    a = a | b;
    return a;
}

CastlingRights operator&(CastlingRights a, CastlingRights b) {
    return static_cast<CastlingRights>(static_cast<U8>(a) & static_cast<U8>(b));
}

CastlingRights& operator&=(CastlingRights& a, CastlingRights b) {
    a = a & b;
    return a;
}

CastlingRights operator~(CastlingRights a) {
    return static_cast<CastlingRights>(~static_cast<U8>(a));
}

bool castlingRightsContains(CastlingRights rights, CastlingRights check) {
    return (rights & check) == check;
}

Color operator-(Color color) {
    if (color == Color::NONE) return Color::NONE;
    return (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

Color getPieceColor(Piece piece) {
    if (isWhitePiece(piece)) {
        return Color::WHITE;
    } else if (isBlackPiece(piece)) {
        return Color::BLACK;
    } else {
        return Color::NONE;
    }
}

bool isBlackPiece(Piece piece) {
    return ((piece & Piece::BLACK) == Piece::BLACK);
}

bool isWhitePiece(Piece piece) {
    return (piece & Piece::WHITE) == Piece::WHITE;
}