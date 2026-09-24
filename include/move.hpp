#pragma once

#include <string>

#include "utils.hpp"

enum class MoveType : U8 {
    QUIET,
    DOUBLE_PUSH,
    CAPTURE,
    QUIET_PROMOTION,
    CAPTURE_PROMOTION,
    EN_PASSANT_CAPTURE,
    CASTLING
};

struct Move {
    MoveType type;

    int fromSquare;
    int toSquare;

    Piece movedPiece;
    Piece capturedPiece;
    Piece promotionPiece;
    CastlingRights castlingType;  // represents the type of castling (kingside
                                  // or queenside) and player color

    static Move quiet(int from, int to, Piece movedPiece);
    static Move doublePush(int from, int to, Piece movedPiece);
    static Move capture(int from, int to, Piece movedPiece, Piece captured);
    static Move quietPromotion(int from, int to, Piece movedPiece,
                               Piece promotion);
    static Move capturePromotion(int from, int to, Piece movedPiece,
                                 Piece promotion, Piece captured);
    static Move enPassant(int from, int to, Piece movedPiece, Piece captured);
    static Move castling(CastlingRights castlingType);

    std::string toString() const;

    bool operator!=(const Move& other) const;
};