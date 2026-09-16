#include "position.hpp"

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
    board[0] = board[7] = Piece::BLACK_ROOK;
    board[1] = board[6] = Piece::BLACK_KNIGHT;
    board[2] = board[5] = Piece::BLACK_BISHOP;
    board[3] = Piece::BLACK_QUEEN;
    board[4] = Piece::BLACK_KING;
    for (int i = 8; i < 16; i++) {
        board[i] = Piece::BLACK_PAWN;
    }

    for (int i = 16; i < 48; i++) {
        board[i] = Piece::EMPTY;
    }

    for (int i = 48; i < 56; i++) {
        board[i] = Piece::WHITE_PAWN;
    }
    board[56] = board[63] = Piece::WHITE_ROOK;
    board[57] = board[62] = Piece::WHITE_KNIGHT;
    board[58] = board[61] = Piece::WHITE_BISHOP;
    board[59] = Piece::WHITE_QUEEN;
    board[60] = Piece::WHITE_KING;

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
            board[index++] = Piece::WHITE_PAWN;
        else if (c == 'N')
            board[index++] = Piece::WHITE_KNIGHT;
        else if (c == 'B')
            board[index++] = Piece::WHITE_BISHOP;
        else if (c == 'R')
            board[index++] = Piece::WHITE_ROOK;
        else if (c == 'Q')
            board[index++] = Piece::WHITE_QUEEN;
        else if (c == 'K')
            board[index++] = Piece::WHITE_KING;
        else if (c == 'p')
            board[index++] = Piece::BLACK_PAWN;
        else if (c == 'n')
            board[index++] = Piece::BLACK_KNIGHT;
        else if (c == 'b')
            board[index++] = Piece::BLACK_BISHOP;
        else if (c == 'r')
            board[index++] = Piece::BLACK_ROOK;
        else if (c == 'q')
            board[index++] = Piece::BLACK_QUEEN;
        else if (c == 'k')
            board[index++] = Piece::BLACK_KING;
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
            else if (piece == Piece::WHITE_PAWN)
                cout << "P ";
            else if (piece == Piece::WHITE_KNIGHT)
                cout << "N ";
            else if (piece == Piece::WHITE_BISHOP)
                cout << "B ";
            else if (piece == Piece::WHITE_ROOK)
                cout << "R ";
            else if (piece == Piece::WHITE_QUEEN)
                cout << "Q ";
            else if (piece == Piece::WHITE_KING)
                cout << "K ";
            else if (piece == Piece::BLACK_PAWN)
                cout << "p ";
            else if (piece == Piece::BLACK_KNIGHT)
                cout << "n ";
            else if (piece == Piece::BLACK_BISHOP)
                cout << "b ";
            else if (piece == Piece::BLACK_ROOK)
                cout << "r ";
            else if (piece == Piece::BLACK_QUEEN)
                cout << "q ";
            else if (piece == Piece::BLACK_KING)
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
    Piece movingPiece = Piece::EMPTY;
    if (move.type != MoveType::CASTLING) {
        movingPiece = board[move.fromSquare];
    }
    bool isPawnMove =
        movingPiece == Piece::WHITE_PAWN || movingPiece == Piece::BLACK_PAWN;
    bool isCapture = move.type == MoveType::CAPTURE ||
                     move.type == MoveType::CAPTURE_PROMOTION ||
                     move.type == MoveType::EN_PASSANT;

    if (isPawnMove || isCapture) {
        halfmoveClock = 0;
    } else {
        halfmoveClock++;
    }

    if (colorToMove == Color::BLACK) fullmoveNumber++;

    switch (move.type) {
        case MoveType::QUIET:
            removeCastlingRights(move);
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::DOUBLE_PUSH:
            enPassantSquare = move.toSquare;
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE:
            removeCastlingRights(move);
            board[move.toSquare] = board[move.fromSquare];
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::QUIET_PROMOTION:
            board[move.toSquare] = move.promotionPiece;
            board[move.fromSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE_PROMOTION:
            removeCastlingRights(move);
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
                    board[60] = Piece::EMPTY;
                    board[61] = Piece::WHITE_ROOK;
                    board[62] = Piece::WHITE_KING;
                    board[63] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::WHITE_QUEENSIDE:
                    board[60] = Piece::EMPTY;
                    board[59] = Piece::WHITE_ROOK;
                    board[58] = Piece::WHITE_KING;
                    board[56] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::WHITE_KINGSIDE |
                                        CastlingRights::WHITE_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_KINGSIDE:
                    board[4] = Piece::EMPTY;
                    board[5] = Piece::BLACK_ROOK;
                    board[6] = Piece::BLACK_KING;
                    board[7] = Piece::EMPTY;
                    castlingRights &= ~(CastlingRights::BLACK_KINGSIDE |
                                        CastlingRights::BLACK_QUEENSIDE);
                    break;
                case CastlingRights::BLACK_QUEENSIDE:
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
                                         ? Piece::WHITE_PAWN
                                         : Piece::BLACK_PAWN;
            board[move.toSquare] = Piece::EMPTY;
            break;

        case MoveType::CAPTURE_PROMOTION:
            board[move.fromSquare] = isWhitePiece(move.promotionPiece)
                                         ? Piece::WHITE_PAWN
                                         : Piece::BLACK_PAWN;
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

    colorToMove = -colorToMove;
}

bool Position::isSquareAttacked(int square, Color byColor) const {
    int r = square / 8, c = square % 8;

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (byColor == Color::WHITE &&
                board[newSquare] == Piece::WHITE_KNIGHT)
                return true;
            if (byColor == Color::BLACK &&
                board[newSquare] == Piece::BLACK_KNIGHT)
                return true;
        }
    }

    for (auto& dir : bishopDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] != Piece::EMPTY) {
                if (byColor == Color::WHITE &&
                    (board[newSquare] == Piece::WHITE_BISHOP ||
                     board[newSquare] == Piece::WHITE_QUEEN))
                    return true;
                if (byColor == Color::BLACK &&
                    (board[newSquare] == Piece::BLACK_BISHOP ||
                     board[newSquare] == Piece::BLACK_QUEEN))
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
                    (board[newSquare] == Piece::WHITE_ROOK ||
                     board[newSquare] == Piece::WHITE_QUEEN) &&
                    getPieceColor(board[newSquare]) == Color::WHITE)
                    return true;
                if (byColor == Color::BLACK &&
                    (board[newSquare] == Piece::BLACK_ROOK ||
                     board[newSquare] == Piece::BLACK_QUEEN) &&
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
                board[newSquare] == Piece::WHITE_KING)
                return true;
            if (byColor == Color::BLACK &&
                board[newSquare] == Piece::BLACK_KING)
                return true;
        }
    }

    if (byColor == Color::WHITE) {
        if (isInsideBoard(r + 1, c - 1) &&
            board[(r + 1) * 8 + (c - 1)] == Piece::WHITE_PAWN)
            return true;
        if (isInsideBoard(r + 1, c + 1) &&
            board[(r + 1) * 8 + (c + 1)] == Piece::WHITE_PAWN)
            return true;
    } else {
        if (isInsideBoard(r - 1, c - 1) &&
            board[(r - 1) * 8 + (c - 1)] == Piece::BLACK_PAWN)
            return true;
        if (isInsideBoard(r - 1, c + 1) &&
            board[(r - 1) * 8 + (c + 1)] == Piece::BLACK_PAWN)
            return true;
    }

    return false;
}

bool Position::inCheck(Color color) const {
    int kingSquare = -1;
    for (int i = 0; i < 64; i++) {
        if (color == Color::WHITE && board[i] == Piece::WHITE_KING) {
            kingSquare = i;
            break;
        }
        if (color == Color::BLACK && board[i] == Piece::BLACK_KING) {
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