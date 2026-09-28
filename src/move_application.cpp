#include "position.hpp"
#include "zobrist_keys.hpp"

void Position::makeMove(const Move& move) {
    Piece movingPiece = Piece::EMPTY;
    if (move.type != MoveType::CASTLING) movingPiece = move.movedPiece;

    bool isPawnMove =
        movingPiece == Piece::WHITE_PAWN || movingPiece == Piece::BLACK_PAWN;
    bool isCapture = move.type == MoveType::CAPTURE ||
                     move.type == MoveType::CAPTURE_PROMOTION ||
                     move.type == MoveType::EN_PASSANT_CAPTURE;

    if (isPawnMove || isCapture) {
        halfmoveClock = 0;
    } else {
        halfmoveClock++;
    }

    if (colorToMove == Color::BLACK) fullmoveNumber++;

    ZobristKeys* zobristKeys = ZobristKeys::getKeys();

    // remove the castling rights from the hash before making the move. If move
    // updates it we'll add the new rights to the hash after making the move, if
    // it doesn't the original rights will be added back to the hash after
    // making the move
    zobristHash ^= zobristKeys->getCastlingRightsKey(castlingRights);

    // regardless of what move type is played the previous enPassant square is
    // going to change
    if (enPassantSquare != -1)
        zobristHash ^= zobristKeys->getEnPassantFileKey(enPassantSquare);

    switch (move.type) {
        case MoveType::QUIET:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^=
                zobristKeys->getPieceSquareKey(move.movedPiece, move.toSquare);

            removeCastlingRights(move);
            board[move.toSquare] = move.movedPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::DOUBLE_PUSH:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^=
                zobristKeys->getPieceSquareKey(move.movedPiece, move.toSquare);

            if (colorToMove == Color::WHITE) {
                enPassantSquare = move.toSquare + 8;
            } else {
                enPassantSquare = move.toSquare - 8;
            }

            // adding the en passant key to the hash for the new en passant
            // square
            zobristHash ^= zobristKeys->getEnPassantFileKey(enPassantSquare);

            board[move.toSquare] = move.movedPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::CAPTURE:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^=
                zobristKeys->getPieceSquareKey(move.movedPiece, move.toSquare);
            zobristHash ^= zobristKeys->getPieceSquareKey(move.capturedPiece,
                                                          move.toSquare);

            removeCastlingRights(move);
            board[move.toSquare] = move.movedPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::QUIET_PROMOTION:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^= zobristKeys->getPieceSquareKey(move.promotionPiece,
                                                          move.toSquare);

            board[move.toSquare] = move.promotionPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::CAPTURE_PROMOTION:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^= zobristKeys->getPieceSquareKey(move.promotionPiece,
                                                          move.toSquare);
            zobristHash ^= zobristKeys->getPieceSquareKey(move.capturedPiece,
                                                          move.toSquare);

            removeCastlingRights(move);
            board[move.toSquare] = move.promotionPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::EN_PASSANT_CAPTURE:
            zobristHash ^= zobristKeys->getPieceSquareKey(move.movedPiece,
                                                          move.fromSquare);
            zobristHash ^=
                zobristKeys->getPieceSquareKey(move.movedPiece, move.toSquare);

            if (colorToMove == Color::WHITE) {
                zobristHash ^= zobristKeys->getPieceSquareKey(
                    move.capturedPiece, enPassantSquare + 8);
                board[enPassantSquare + 8] = Piece::EMPTY;
            } else {
                zobristHash ^= zobristKeys->getPieceSquareKey(
                    move.capturedPiece, enPassantSquare - 8);
                board[enPassantSquare - 8] = Piece::EMPTY;
            }

            board[enPassantSquare] = move.movedPiece;
            board[move.fromSquare] = Piece::EMPTY;

            break;

        case MoveType::CASTLING:
            switch (move.castlingType) {
                case CastlingRights::WHITE_KINGSIDE:
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_KING, 60);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_KING, 62);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_ROOK, 63);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_ROOK, 61);

                    board[60] = Piece::EMPTY;
                    board[61] = Piece::WHITE_ROOK;
                    board[62] = Piece::WHITE_KING;
                    board[63] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::WHITE_QUEENSIDE:
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_KING, 60);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_KING, 58);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_ROOK, 56);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::WHITE_ROOK, 59);

                    board[60] = Piece::EMPTY;
                    board[59] = Piece::WHITE_ROOK;
                    board[58] = Piece::WHITE_KING;
                    board[56] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_KINGSIDE:
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_KING, 4);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_KING, 6);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_ROOK, 7);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_ROOK, 5);

                    board[4] = Piece::EMPTY;
                    board[5] = Piece::BLACK_ROOK;
                    board[6] = Piece::BLACK_KING;
                    board[7] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::BLACK_KINGSIDE |
                                        CastlingRights::BLACK_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_QUEENSIDE:
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_KING, 4);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_KING, 2);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_ROOK, 0);
                    zobristHash ^=
                        zobristKeys->getPieceSquareKey(Piece::BLACK_ROOK, 3);

                    board[4] = Piece::EMPTY;
                    board[3] = Piece::BLACK_ROOK;
                    board[2] = Piece::BLACK_KING;
                    board[0] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::BLACK_KINGSIDE |
                                        CastlingRights::BLACK_QUEENSIDE);
                    break;
            }
            break;
        default:
            break;
    }

    // adding back castling rights (they might have been updated by the move)
    zobristHash ^= zobristKeys->getCastlingRightsKey(castlingRights);

    // if this was white's move we remove the color to move key, if it was
    // black's move we add it back
    zobristHash ^= zobristKeys->getColorToMoveKey();

    if (move.type != MoveType::DOUBLE_PUSH) enPassantSquare = -1;

    colorToMove = -colorToMove;
}

void Position::makeMove(const Move& move, StateInfo& saveState) {
    saveState.castlingRights = castlingRights;
    saveState.enPassantSquare = enPassantSquare;
    saveState.halfmoveClock = halfmoveClock;
    saveState.fullmoveNumber = fullmoveNumber;
    saveState.zobristHash = zobristHash;

    makeMove(move);
}

void Position::undoMove(const Move& move, const StateInfo& savedState) {
    castlingRights = savedState.castlingRights;
    enPassantSquare = savedState.enPassantSquare;
    halfmoveClock = savedState.halfmoveClock;
    fullmoveNumber = savedState.fullmoveNumber;
    zobristHash = savedState.zobristHash;
    colorToMove = -colorToMove;

    switch (move.type) {
        case MoveType::QUIET:
        case MoveType::DOUBLE_PUSH:
        case MoveType::QUIET_PROMOTION:
            board[move.fromSquare] = move.movedPiece;
            board[move.toSquare] = Piece::EMPTY;

            break;

        case MoveType::CAPTURE:
        case MoveType::CAPTURE_PROMOTION:
            board[move.fromSquare] = move.movedPiece;
            board[move.toSquare] = move.capturedPiece;

            break;

        case MoveType::EN_PASSANT_CAPTURE:
            board[move.fromSquare] = move.movedPiece;
            board[move.toSquare] = Piece::EMPTY;
            if (colorToMove == Color::WHITE)
                board[savedState.enPassantSquare + 8] = move.capturedPiece;
            else
                board[savedState.enPassantSquare - 8] = move.capturedPiece;

            break;

        case MoveType::CASTLING:
            switch (move.castlingType) {
                case CastlingRights::WHITE_KINGSIDE:
                    board[60] = Piece::WHITE_KING;
                    board[61] = Piece::EMPTY;
                    board[62] = Piece::EMPTY;
                    board[63] = Piece::WHITE_ROOK;
                    break;

                case CastlingRights::WHITE_QUEENSIDE:
                    board[60] = Piece::WHITE_KING;
                    board[59] = Piece::EMPTY;
                    board[58] = Piece::EMPTY;
                    board[56] = Piece::WHITE_ROOK;
                    break;

                case CastlingRights::BLACK_KINGSIDE:
                    board[4] = Piece::BLACK_KING;
                    board[5] = Piece::EMPTY;
                    board[6] = Piece::EMPTY;
                    board[7] = Piece::BLACK_ROOK;
                    break;

                case CastlingRights::BLACK_QUEENSIDE:
                    board[4] = Piece::BLACK_KING;
                    board[3] = Piece::EMPTY;
                    board[2] = Piece::EMPTY;
                    board[0] = Piece::BLACK_ROOK;
                    break;

                default:
                    break;
            }

            break;
        default:
            break;
    }
}
