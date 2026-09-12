#include "position.h"

#include <iostream>

using namespace std;

Position::Position() { initPos(); }

Position::Position(const string& fen) { parseFENPos(fen); }

Piece Position::getPieceAt(int square) const { return board[square]; }

Color Position::getColorToMove() const { return colorToMove; }

CastlingRights Position::getCastlingRights() const { return castlingRights; }

int Position::getEnPassantSquare() const { return enPassantSquare; }

int Position::getHalfmoveClock() const { return halfmoveClock; }

int Position::getFullmoveNumber() const { return fullmoveNumber; }

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

string Position::toFEN() const {
    // Implement FEN generation logic here
    return "";
}

bool Position::isSquareAttacked(int square, Color byColor) const {
    // Implement logic to check if a square is attacked by a piece of the given
    // color
    return false;
}

bool Position::inCheck(Color color) const {
    for (int i = 0; i < 64; i++) {
        if ((color == Color::WHITE &&
             board[i] == (Piece::KING | Piece::WHITE)) ||
            (color == Color::BLACK &&
             board[i] == (Piece::KING | Piece::BLACK))) {
            return isSquareAttacked(i, -color);
        }
    }
    return false;
}