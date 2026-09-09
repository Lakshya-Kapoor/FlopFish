#include <iostream>
using namespace std;

enum Piece {
    EMPTY,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

void initBoard(Piece board[]) {
    board[0] = board[7] = BLACK_ROOK;
    board[1] = board[6] = BLACK_KNIGHT;
    board[2] = board[5] = BLACK_BISHOP;
    board[3] = BLACK_QUEEN;
    board[4] = BLACK_KING;
    for (int i = 8; i < 16; i++) board[i] = BLACK_PAWN;

    board[56] = board[63] = WHITE_ROOK;
    board[57] = board[62] = WHITE_KNIGHT;
    board[58] = board[61] = WHITE_BISHOP;
    board[59] = WHITE_QUEEN;
    board[60] = WHITE_KING;
    for (int i = 48; i < 56; i++) board[i] = WHITE_PAWN;

    for (int i = 16; i < 48; i++) board[i] = EMPTY;
}

void printBoard(Piece board[]) {
    for (int i = 0; i < 8; i++) {
        cout << 8 - i << " ";
        for (int j = 0; j < 8; j++) {
            Piece piece = board[i * 8 + j];
            if (piece == EMPTY)
                cout << ". ";
            else if (piece == WHITE_PAWN)
                cout << "P ";
            else if (piece == WHITE_KNIGHT)
                cout << "N ";
            else if (piece == WHITE_BISHOP)
                cout << "B ";
            else if (piece == WHITE_ROOK)
                cout << "R ";
            else if (piece == WHITE_QUEEN)
                cout << "Q ";
            else if (piece == WHITE_KING)
                cout << "K ";
            else if (piece == BLACK_PAWN)
                cout << "p ";
            else if (piece == BLACK_KNIGHT)
                cout << "n ";
            else if (piece == BLACK_BISHOP)
                cout << "b ";
            else if (piece == BLACK_ROOK)
                cout << "r ";
            else if (piece == BLACK_QUEEN)
                cout << "q ";
            else if (piece == BLACK_KING)
                cout << "k ";
        }
        cout << endl;
    }
    cout << "  a b c d e f g h" << endl;
}

void parseFEN(string fen, Piece board[]) {
    int index = 0;
    for (char c : fen) {
        if (c == '/') continue;
        if (isdigit(c)) {
            int emptySquares = c - '0';
            for (int i = 0; i < emptySquares; i++) {
                board[index++] = EMPTY;
            }
        } else if (c == 'P')
            board[index++] = WHITE_PAWN;
        else if (c == 'N')
            board[index++] = WHITE_KNIGHT;
        else if (c == 'B')
            board[index++] = WHITE_BISHOP;
        else if (c == 'R')
            board[index++] = WHITE_ROOK;
        else if (c == 'Q')
            board[index++] = WHITE_QUEEN;
        else if (c == 'K')
            board[index++] = WHITE_KING;
        else if (c == 'p')
            board[index++] = BLACK_PAWN;
        else if (c == 'n')
            board[index++] = BLACK_KNIGHT;
        else if (c == 'b')
            board[index++] = BLACK_BISHOP;
        else if (c == 'r')
            board[index++] = BLACK_ROOK;
        else if (c == 'q')
            board[index++] = BLACK_QUEEN;
        else if (c == 'k')
            board[index++] = BLACK_KING;
    }
}

int main() {
    while (true) {
        Piece board[64];
        string fen;
        cout << "Enter FEN string: ";
        getline(cin, fen);
        if (fen == "exit") break;

        parseFEN(fen, board);
        printBoard(board);
    }
    return 0;
}