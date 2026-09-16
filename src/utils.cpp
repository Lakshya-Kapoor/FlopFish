#include "utils.hpp"

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
    return piece >= Piece::BLACK_PAWN && piece <= Piece::BLACK_KING;
}

bool isWhitePiece(Piece piece) {
    return piece >= Piece::WHITE_PAWN && piece <= Piece::WHITE_KING;
}

int getPieceValue(Piece piece) {
    switch (piece) {
        case Piece::EMPTY:
            return 0;
        case Piece::WHITE_PAWN:
        case Piece::BLACK_PAWN:
            return 100;
        case Piece::WHITE_KNIGHT:
        case Piece::BLACK_KNIGHT:
            return 320;
        case Piece::WHITE_BISHOP:
        case Piece::BLACK_BISHOP:
            return 330;
        case Piece::WHITE_ROOK:
        case Piece::BLACK_ROOK:
            return 500;
        case Piece::WHITE_QUEEN:
        case Piece::BLACK_QUEEN:
            return 900;
        case Piece::WHITE_KING:
        case Piece::BLACK_KING:
            return 20000;
        default:
            return 0;
    }
}