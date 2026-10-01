#include "game_state.hpp"

#include <iostream>
#include <sstream>
#include <vector>

#include "zobrist_keys.hpp"

using namespace std;

LegalityInfo::LegalityInfo(int kingSquare, bool inCheck, Pin pinnedSquare[],
                           bool evasionSquares[]) {
    this->kingSquare = kingSquare;
    this->inCheck = inCheck;
    for (int i = 0; i < 64; i++) {
        this->pinnedSquare[i] = pinnedSquare[i];
        this->evasionSquares[i] = evasionSquares[i];
    }
}

bool LegalityInfo::legalityRespected(int fromSquare, int toSquare) const {
    if (pinnedSquare[fromSquare].isPinned) {
        int fromR = fromSquare / 8, fromC = fromSquare % 8;
        int toR = toSquare / 8, toC = toSquare % 8;

        int dr = pinnedSquare[fromSquare].dr;
        int dc = pinnedSquare[fromSquare].dc;

        if (dc == 0) {
            if (fromC != toC) return false;
        } else if (dr == 0) {
            if (fromR != toR) return false;
        } else {
            if ((fromR - toR) * dc != (fromC - toC) * dr) {
                return false;
            }
        }
    }

    if (inCheck) {
        if (!evasionSquares[toSquare]) {
            return false;  // Move does not evade the check
        }
    }

    return true;
}

GameState::GameState() {
    initPos();
    zobristHash = generateZobristHash();
    positionCount[zobristHash] = 1;
}

GameState::GameState(const string& fen) {
    parseFENPos(fen);
    zobristHash = generateZobristHash();
    positionCount[zobristHash] = 1;
}

Piece GameState::getPieceAt(int square) const { return board[square]; }

Color GameState::getColorToMove() const { return colorToMove; }

CastlingRights GameState::getCastlingRights() const { return castlingRights; }

int GameState::getEnPassantSquare() const { return enPassantSquare; }

int GameState::getHalfmoveClock() const { return halfmoveClock; }

int GameState::getFullmoveNumber() const { return fullmoveNumber; }

PositionState GameState::getPositionState() {
    vector<Move> legalMoves = generateLegalMoves();
    return getPositionState(legalMoves);
}

PositionState GameState::getPositionState(vector<Move>& legalMoves) {
    if (positionCount[zobristHash] >= 3)
        return PositionState::DRAW_BY_REPETITION;

    if (halfmoveClock >= 100) return PositionState::DRAW_BY_HALFCLOCK;

    if (legalMoves.empty()) {
        if (inCheck(colorToMove)) {
            return PositionState::CHECKMATE;
        } else {
            return PositionState::STALEMATE;
        }
    }

    return PositionState::ONGOING;
}

U64 GameState::getZobristHash() const { return zobristHash; }

void GameState::initPos() {
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

void GameState::parseFENPos(const string& fen) {
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

U64 GameState::generateZobristHash() {
    ZobristKeys* zobristKeys = ZobristKeys::getKeys();
    U64 hash = 0;

    for (int square = 0; square < 64; square++) {
        Piece piece = board[square];
        if (piece != Piece::EMPTY) {
            int pieceIndex = static_cast<int>(piece) - 1;
            hash ^= zobristKeys->getPieceSquareKey(piece, square);
        }
    }

    // Add color to move key only for white
    if (colorToMove == Color::WHITE) {
        hash ^= zobristKeys->getColorToMoveKey();
    }

    hash ^= zobristKeys->getCastlingRightsKey(castlingRights);

    if (enPassantSquare != -1) {
        int file = enPassantSquare % 8;
        hash ^= zobristKeys->getEnPassantKey(file);
    }

    return hash;
}

void GameState::print() const {
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

bool GameState::isInsideBoard(int r, int c) const {
    return r >= 0 && r < 8 && c >= 0 && c < 8;
}

bool GameState::isSquareAttacked(int square, Color byColor) const {
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

bool GameState::inCheck(Color color) const {
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

void GameState::removeCastlingRights(const Move& move) {
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