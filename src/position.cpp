#include "../include/position.hpp"

#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Position::Position() { initPos(); }

Position::Position(const string& fen) { parseFENPos(fen); }

Piece Position::getPieceAt(int square) const { return board[square]; }

Color Position::getColorToMove() const { return colorToMove; }

CastlingRights Position::getCastlingRights() const { return castlingRights; }

int Position::getEnPassantSquare() const { return enPassantSquare; }

int Position::getHalfmoveClock() const { return halfmoveClock; }

int Position::getFullmoveNumber() const { return fullmoveNumber; }

PositionState Position::getPositionState() {
    vector<Move> legalMoves = generateLegalMoves();

    if (legalMoves.empty()) {
        if (inCheck(colorToMove)) {
            return PositionState::CHECKMATE;
        } else {
            return PositionState::STALEMATE;
        }
    }

    if (halfmoveClock >= 100) {
        return PositionState::DRAW;
    }

    return PositionState::ONGOING;
}

void Position::initPos() {
    board[0] = board[7] = Piece::ROOK | Piece::BLACK;
    board[1] = board[6] = Piece::KNIGHT | Piece::BLACK;
    board[2] = board[5] = Piece::BISHOP | Piece::BLACK;
    board[3] = Piece::QUEEN | Piece::BLACK;
    board[4] = Piece::KING | Piece::BLACK;
    for (int i = 8; i < 16; i++) {
        board[i] = Piece::PAWN | Piece::BLACK;
    }

    for (int i = 16; i < 48; i++) {
        board[i] = Piece::EMPTY;
    }

    for (int i = 48; i < 56; i++) {
        board[i] = Piece::PAWN | Piece::WHITE;
    }
    board[56] = board[63] = Piece::ROOK | Piece::WHITE;
    board[57] = board[62] = Piece::KNIGHT | Piece::WHITE;
    board[58] = board[61] = Piece::BISHOP | Piece::WHITE;
    board[59] = Piece::QUEEN | Piece::WHITE;
    board[60] = Piece::KING | Piece::WHITE;

    colorToMove = Color::WHITE;
    castlingRights =
        CastlingRights::WHITE_KINGSIDE | CastlingRights::WHITE_QUEENSIDE |
        CastlingRights::BLACK_KINGSIDE | CastlingRights::BLACK_QUEENSIDE;
    enPassantSquare = -1;
    halfmoveClock = 0;
    fullmoveNumber = 1;
}

void Position::parseFENPos(const string& fen) {
    istringstream iss(fen);
    string boardPart, colorPart, castlingPart, enPassantPart, halfmovePart,
        fullmovePart;

    iss >> boardPart >> colorPart >> castlingPart >> enPassantPart >>
        halfmovePart >> fullmovePart;

    int index = 0;
    for (char c : boardPart) {
        if (c == '/') continue;
        if (isdigit(c)) {
            int emptySquares = c - '0';
            for (int i = 0; i < emptySquares; i++) {
                board[index++] = Piece::EMPTY;
            }
        } else if (c == 'P')
            board[index++] = Piece::PAWN | Piece::WHITE;
        else if (c == 'N')
            board[index++] = Piece::KNIGHT | Piece::WHITE;
        else if (c == 'B')
            board[index++] = Piece::BISHOP | Piece::WHITE;
        else if (c == 'R')
            board[index++] = Piece::ROOK | Piece::WHITE;
        else if (c == 'Q')
            board[index++] = Piece::QUEEN | Piece::WHITE;
        else if (c == 'K')
            board[index++] = Piece::KING | Piece::WHITE;
        else if (c == 'p')
            board[index++] = Piece::PAWN | Piece::BLACK;
        else if (c == 'n')
            board[index++] = Piece::KNIGHT | Piece::BLACK;
        else if (c == 'b')
            board[index++] = Piece::BISHOP | Piece::BLACK;
        else if (c == 'r')
            board[index++] = Piece::ROOK | Piece::BLACK;
        else if (c == 'q')
            board[index++] = Piece::QUEEN | Piece::BLACK;
        else if (c == 'k')
            board[index++] = Piece::KING | Piece::BLACK;
    }

    colorToMove = (colorPart == "w") ? Color::WHITE : Color::BLACK;

    castlingRights = CastlingRights::NONE;
    for (char c : castlingPart) {
        if (c == 'K')
            castlingRights = castlingRights | CastlingRights::WHITE_KINGSIDE;
        else if (c == 'Q')
            castlingRights = castlingRights | CastlingRights::WHITE_QUEENSIDE;
        else if (c == 'k')
            castlingRights = castlingRights | CastlingRights::BLACK_KINGSIDE;
        else if (c == 'q')
            castlingRights = castlingRights | CastlingRights::BLACK_QUEENSIDE;
    }

    if (enPassantPart == "-")
        enPassantSquare = -1;
    else {
        int file = enPassantPart[0] - 'a';
        int rank = 8 - (enPassantPart[1] - '0');
        enPassantSquare = rank * 8 + file;
    }

    halfmoveClock = stoi(halfmovePart);
    fullmoveNumber = stoi(fullmovePart);
}

void Position::print() const {
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            Piece piece = board[i * 8 + j];
            if (piece == Piece::EMPTY)
                cout << ". ";
            else if (piece == (Piece::PAWN | Piece::WHITE))
                cout << "P ";
            else if (piece == (Piece::KNIGHT | Piece::WHITE))
                cout << "N ";
            else if (piece == (Piece::BISHOP | Piece::WHITE))
                cout << "B ";
            else if (piece == (Piece::ROOK | Piece::WHITE))
                cout << "R ";
            else if (piece == (Piece::QUEEN | Piece::WHITE))
                cout << "Q ";
            else if (piece == (Piece::KING | Piece::WHITE))
                cout << "K ";
            else if (piece == (Piece::PAWN | Piece::BLACK))
                cout << "p ";
            else if (piece == (Piece::KNIGHT | Piece::BLACK))
                cout << "n ";
            else if (piece == (Piece::BISHOP | Piece::BLACK))
                cout << "b ";
            else if (piece == (Piece::ROOK | Piece::BLACK))
                cout << "r ";
            else if (piece == (Piece::QUEEN | Piece::BLACK))
                cout << "q ";
            else if (piece == (Piece::KING | Piece::BLACK))
                cout << "k ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

bool Position::isInsideBoard(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

void Position::makeMove(const Move& move) {
    if (move.type != MoveType::CASTLING &&
        (move.type == MoveType::CAPTURE ||
         (board[move.fromSquare] & Piece::PAWN) == Piece::PAWN)) {
        halfmoveClock = 0;
    } else {
        halfmoveClock++;
    }

    if (colorToMove == Color::BLACK) fullmoveNumber++;

    switch (move.type) {
        case MoveType::QUIET:
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::DOUBLE_PUSH:
            enPassantSquare = move.toSquare;
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE:
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::QUIET_PROMOTION:
            board[move.toSquare] = move.promotionPiece;
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE_PROMOTION:
            board[move.toSquare] = move.promotionPiece;
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::EN_PASSANT:
            board[enPassantSquare] = Piece::EMPTY;
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::CASTLING:
            switch (move.castlingType) {
                case CastlingRights::WHITE_KINGSIDE:
                    board[4] = Piece::EMPTY;
                    board[5] = Piece::ROOK | Piece::WHITE;
                    board[6] = Piece::KING | Piece::WHITE;
                    board[7] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::WHITE_QUEENSIDE:
                    board[4] = Piece::EMPTY;
                    board[3] = Piece::ROOK | Piece::WHITE;
                    board[2] = Piece::KING | Piece::WHITE;
                    board[0] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_KINGSIDE:
                    board[60] = Piece::EMPTY;
                    board[61] = Piece::ROOK | Piece::BLACK;
                    board[62] = Piece::KING | Piece::BLACK;
                    board[63] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::BLACK_KINGSIDE |
                                        CastlingRights::BLACK_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_QUEENSIDE:
                    board[60] = Piece::EMPTY;
                    board[59] = Piece::ROOK | Piece::BLACK;
                    board[58] = Piece::KING | Piece::BLACK;
                    board[56] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::BLACK_KINGSIDE |
                                        CastlingRights::BLACK_QUEENSIDE);
                    break;
            }
            break;
        default:
            break;
    }

    if (move.type != MoveType::DOUBLE_PUSH) enPassantSquare = -1;

    colorToMove = -colorToMove;
}

void Position::makeMove(const Move& move, StateInfo& saveState) {
    saveState.castlingRights = castlingRights;
    saveState.enPassantSquare = enPassantSquare;
    saveState.halfmoveClock = halfmoveClock;
    saveState.fullmoveNumber = fullmoveNumber;

    makeMove(move);
}

void Position::undoMove(const Move& move, const StateInfo& savedState) {
    castlingRights = savedState.castlingRights;
    enPassantSquare = savedState.enPassantSquare;
    halfmoveClock = savedState.halfmoveClock;
    fullmoveNumber = savedState.fullmoveNumber;

    switch (move.type) {
        case MoveType::QUIET:
            board[move.fromSquare] = board[move.toSquare];
            board[move.toSquare] = Piece::EMPTY;
            break;

        case MoveType::DOUBLE_PUSH:
            board[move.fromSquare] = board[move.toSquare];
            board[move.toSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE:
            board[move.fromSquare] = board[move.toSquare];
            board[move.toSquare] = move.capturedPiece;
            break;

        case MoveType::QUIET_PROMOTION:
            board[move.fromSquare] = isWhitePiece(move.promotionPiece)
                                         ? Piece::PAWN | Piece::WHITE
                                         : Piece::PAWN | Piece::BLACK;
            board[move.toSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE_PROMOTION:
            board[move.fromSquare] = isWhitePiece(move.promotionPiece)
                                         ? Piece::PAWN | Piece::WHITE
                                         : Piece::PAWN | Piece::BLACK;
            board[move.toSquare] = move.capturedPiece;
            break;

        case MoveType::EN_PASSANT:
            board[move.fromSquare] = board[move.toSquare];
            board[move.toSquare] = Piece::EMPTY;
            board[savedState.enPassantSquare] = move.capturedPiece;
            break;

        case MoveType::CASTLING:
            switch (move.castlingType) {
                case CastlingRights::WHITE_KINGSIDE:
                    board[4] = Piece::KING | Piece::WHITE;
                    board[5] = Piece::EMPTY;
                    board[6] = Piece::EMPTY;
                    board[7] = Piece::ROOK | Piece::WHITE;
                    break;

                case CastlingRights::WHITE_QUEENSIDE:
                    board[4] = Piece::KING | Piece::WHITE;
                    board[3] = Piece::EMPTY;
                    board[2] = Piece::EMPTY;
                    board[0] = Piece::ROOK | Piece::WHITE;
                    break;

                case CastlingRights::BLACK_KINGSIDE:
                    board[60] = Piece::KING | Piece::BLACK;
                    board[61] = Piece::EMPTY;
                    board[62] = Piece::EMPTY;
                    board[63] = Piece::ROOK | Piece::BLACK;
                    break;

                case CastlingRights::BLACK_QUEENSIDE:
                    board[60] = Piece::KING | Piece::BLACK;
                    board[59] = Piece::EMPTY;
                    board[58] = Piece::EMPTY;
                    board[56] = Piece::ROOK | Piece::BLACK;
                    break;

                default:
                    break;
            }

            break;
        default:
            break;
    }

    colorToMove = -colorToMove;
}

bool Position::isSquareAttacked(int square, Color byColor) const {
    int r = square / 8, c = square % 8;

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (byColor == Color::WHITE &&
                board[newSquare] == (Piece::KNIGHT | Piece::WHITE))
                return true;
            if (byColor == Color::BLACK &&
                board[newSquare] == (Piece::KNIGHT | Piece::BLACK))
                return true;
        }
    }

    for (auto& dir : bishopDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] != Piece::EMPTY) {
                if (byColor == Color::WHITE &&
                    (board[newSquare] == (Piece::BISHOP | Piece::WHITE) ||
                     board[newSquare] == (Piece::QUEEN | Piece::WHITE)))
                    return true;
                if (byColor == Color::BLACK &&
                    (board[newSquare] == (Piece::BISHOP | Piece::BLACK) ||
                     board[newSquare] == (Piece::QUEEN | Piece::BLACK)))
                    return true;
                break;
            }
            newR += dir[0];
            newC += dir[1];
        }
    }

    for (auto& dir : rookDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] != Piece::EMPTY) {
                if (byColor == Color::WHITE &&
                    (board[newSquare] == (Piece::ROOK | Piece::WHITE) ||
                     board[newSquare] == (Piece::QUEEN | Piece::WHITE)) &&
                    getPieceColor(board[newSquare]) == Color::WHITE)
                    return true;
                if (byColor == Color::BLACK &&
                    (board[newSquare] == (Piece::ROOK | Piece::BLACK) ||
                     board[newSquare] == (Piece::QUEEN | Piece::BLACK)) &&
                    getPieceColor(board[newSquare]) == Color::BLACK)
                    return true;
                break;
            }
            newR += dir[0];
            newC += dir[1];
        }
    }

    for (auto& dir : kingDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (byColor == Color::WHITE &&
                board[newSquare] == (Piece::KING | Piece::WHITE))
                return true;
            if (byColor == Color::BLACK &&
                board[newSquare] == (Piece::KING | Piece::BLACK))
                return true;
        }
    }

    if (byColor == Color::WHITE) {
        if (isInsideBoard(r - 1, c - 1) &&
            board[(r - 1) * 8 + (c - 1)] == (Piece::PAWN | Piece::WHITE))
            return true;
        if (isInsideBoard(r - 1, c + 1) &&
            board[(r - 1) * 8 + (c + 1)] == (Piece::PAWN | Piece::WHITE))
            return true;
    } else {
        if (isInsideBoard(r + 1, c - 1) &&
            board[(r + 1) * 8 + (c - 1)] == (Piece::PAWN | Piece::BLACK))
            return true;
        if (isInsideBoard(r + 1, c + 1) &&
            board[(r + 1) * 8 + (c + 1)] == (Piece::PAWN | Piece::BLACK))
            return true;
    }

    return false;
}

bool Position::inCheck(Color color) const {
    int kingSquare = -1;
    for (int i = 0; i < 64; i++) {
        if (color == Color::WHITE && board[i] == (Piece::KING | Piece::WHITE)) {
            kingSquare = i;
            break;
        }
        if (color == Color::BLACK && board[i] == (Piece::KING | Piece::BLACK)) {
            kingSquare = i;
            break;
        }
    }

    return isSquareAttacked(kingSquare, -color);
}

void Position::removeCastlingRights(const Move& move) {
    if (castlingRightsContains(castlingRights,
                               CastlingRights::WHITE_KINGSIDE) &&
        (move.fromSquare == 63 || move.fromSquare == 60 ||
         move.toSquare == 63)) {
        castlingRights &= ~CastlingRights::WHITE_KINGSIDE;
    }
    if (castlingRightsContains(castlingRights,
                               CastlingRights::WHITE_QUEENSIDE) &&
        (move.fromSquare == 56 || move.fromSquare == 60 ||
         move.toSquare == 56)) {
        castlingRights &= ~CastlingRights::WHITE_QUEENSIDE;
    }
    if (castlingRightsContains(castlingRights,
                               CastlingRights::BLACK_KINGSIDE) &&
        (move.fromSquare == 7 || move.fromSquare == 4 || move.toSquare == 7)) {
        castlingRights &= ~CastlingRights::BLACK_KINGSIDE;
    }
    if (castlingRightsContains(castlingRights,
                               CastlingRights::BLACK_QUEENSIDE) &&
        (move.fromSquare == 0 || move.fromSquare == 4 || move.toSquare == 0)) {
        castlingRights &= ~CastlingRights::BLACK_QUEENSIDE;
    }
}