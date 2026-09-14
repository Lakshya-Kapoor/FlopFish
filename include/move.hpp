#pragma once

#include <string>

#include "utils.hpp"

enum class MoveType : U8 {
    QUIET,
    DOUBLE_PUSH,
    CAPTURE,
    QUIET_PROMOTION,
    CAPTURE_PROMOTION,
    EN_PASSANT,
    CASTLING
};

struct Move {
    MoveType type;

    int fromSquare;
    int toSquare;

    Piece promotionPiece;
    Piece capturedPiece;
    CastlingRights castlingType;

    static Move quiet(int from, int to);
    static Move doublePush(int from, int to);
    static Move capture(int from, int to, Piece captured);
    static Move quietPromotion(int from, int to, Piece promotion);
    static Move capturePromotion(int from, int to, Piece promotion,
                                 Piece captured);
    static Move enPassant(int from, int to, Piece captured);
    static Move castling(CastlingRights castlingType);

    std::string toString() const;
};