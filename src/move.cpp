#include "../include/move.hpp"

#include <string>

using namespace std;

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

string Move::toString() const {
    string moveStr;

    if (type == MoveType::CASTLING) {
        if (castlingType == CastlingRights::WHITE_KINGSIDE) return "e1g1";
        if (castlingType == CastlingRights::WHITE_QUEENSIDE) return "e1c1";
        if (castlingType == CastlingRights::BLACK_KINGSIDE) return "e8g8";
        if (castlingType == CastlingRights::BLACK_QUEENSIDE) return "e8c8";
    }

    int fromRank = fromSquare / 8;
    int fromFile = fromSquare % 8;
    int toRank = toSquare / 8;
    int toFile = toSquare % 8;

    moveStr += ('a' + fromFile);
    moveStr += (8 - fromRank) + '0';
    moveStr += ('a' + toFile);
    moveStr += (8 - toRank) + '0';

    if (type == MoveType::QUIET_PROMOTION ||
        type == MoveType::CAPTURE_PROMOTION) {
        switch (promotionPiece) {
            case Piece::WHITE_QUEEN:
            case Piece::BLACK_QUEEN:
                moveStr += 'q';
                break;
            case Piece::WHITE_ROOK:
            case Piece::BLACK_ROOK:
                moveStr += 'r';
                break;
            case Piece::WHITE_BISHOP:
            case Piece::BLACK_BISHOP:
                moveStr += 'b';
                break;
            case Piece::WHITE_KNIGHT:
            case Piece::BLACK_KNIGHT:
                moveStr += 'n';
                break;
            default:
                break;
        }
    }

    return moveStr;
}
