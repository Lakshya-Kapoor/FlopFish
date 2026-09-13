#include "../include/move.hpp"

Move Move::quiet(int from, int to) {
    Move m;
    m.type = MoveType::QUIET;
    m.fromSquare = from;
    m.toSquare = to;
    return m;
}

Move Move::doublePush(int from, int to) {
    Move m;
    m.type = MoveType::DOUBLE_PUSH;
    m.fromSquare = from;
    m.toSquare = to;
    return m;
}

Move Move::capture(int from, int to, Piece captured) {
    Move m;
    m.type = MoveType::CAPTURE;
    m.fromSquare = from;
    m.toSquare = to;
    m.capturedPiece = captured;
    return m;
}

Move Move::quietPromotion(int from, int to, Piece promotion) {
    Move m;
    m.type = MoveType::QUIET_PROMOTION;
    m.fromSquare = from;
    m.toSquare = to;
    m.promotionPiece = promotion;
    return m;
}

Move Move::capturePromotion(int from, int to, Piece promotion, Piece captured) {
    Move m;
    m.type = MoveType::CAPTURE_PROMOTION;
    m.fromSquare = from;
    m.toSquare = to;
    m.promotionPiece = promotion;
    m.capturedPiece = captured;
    return m;
}

Move Move::enPassant(int from, int to, Piece captured) {
    Move m;
    m.type = MoveType::EN_PASSANT;
    m.fromSquare = from;
    m.toSquare = to;
    m.capturedPiece = captured;
    return m;
}

Move Move::castling(CastlingRights castlingType) {
    Move m;
    m.type = MoveType::CASTLING;
    m.castlingType = castlingType;
    return m;
}