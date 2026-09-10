#include <iostream>
#include <vector>
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

struct Move {
    int from;
    int to;
};

void initBoard(vector<Piece>& board) {
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

void printBoard(vector<Piece>& board) {
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

void parseFEN(string fen, vector<Piece>& board) {
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

bool isWhitePiece(Piece piece) {
    return piece >= WHITE_PAWN && piece <= WHITE_KING;
}

bool isBlackPiece(Piece piece) {
    return piece >= BLACK_PAWN && piece <= BLACK_KING;
}

int pieceColor(Piece piece) {
    if (isWhitePiece(piece)) return 1;
    if (isBlackPiece(piece)) return -1;
    return 0;  // Empty square
}

bool isValidSquare(int square) { return square >= 0 && square < 64; }

bool isInsideBoard(int r, int c) { return r >= 0 && r < 8 && c >= 0 && c < 8; }

void generateKnightMoves(int square, vector<Piece>& board,
                         vector<Move>& moves) {
    int knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                             {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

    int r = square / 8, c = square % 8;
    for (auto& move : knightMoves) {
        int newR = r + move[0], newC = c + move[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (pieceColor(board[newSquare]) != pieceColor(board[square])) {
                moves.push_back({square, newSquare});
            }
        }
    }
}

void generateBishopMoves(int square, vector<Piece>& board,
                         vector<Move>& moves) {
    int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    int r = square / 8, c = square % 8;

    for (auto& dir : directions) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == EMPTY) {
                moves.push_back({square, newSquare});
            } else {
                if (pieceColor(board[newSquare]) != pieceColor(board[square])) {
                    moves.push_back({square, newSquare});
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void generateRookMoves(int square, vector<Piece>& board, vector<Move>& moves) {
    int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int r = square / 8, c = square % 8;

    for (auto& dir : directions) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == EMPTY) {
                moves.push_back({square, newSquare});
            } else {
                if (pieceColor(board[newSquare]) != pieceColor(board[square])) {
                    moves.push_back({square, newSquare});
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void generateQueenMoves(int square, vector<Piece>& board, vector<Move>& moves) {
    generateBishopMoves(square, board, moves);
    generateRookMoves(square, board, moves);
}

void generateKingMoves(int square, vector<Piece>& board, vector<Move>& moves) {
    int kingMoves[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                           {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

    int r = square / 8, c = square % 8;
    for (auto& move : kingMoves) {
        int newR = r + move[0], newC = c + move[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (pieceColor(board[newSquare]) != pieceColor(board[square])) {
                moves.push_back({square, newSquare});
            }
        }
    }
}

void generatePawnMoves(int square, vector<Piece>& board, vector<Move>& moves) {
    int r = square / 8, c = square % 8;
    Piece piece = board[square];

    if (piece == WHITE_PAWN) {
        // Move forward
        if (isInsideBoard(r - 1, c) && board[(r - 1) * 8 + c] == EMPTY) {
            moves.push_back({square, (r - 1) * 8 + c});
            // Double move from starting position
            if (r == 6 && board[(r - 2) * 8 + c] == EMPTY) {
                moves.push_back({square, (r - 2) * 8 + c});
            }
        }
        // Capture diagonally
        if (isInsideBoard(r - 1, c - 1) &&
            isBlackPiece(board[(r - 1) * 8 + (c - 1)])) {
            moves.push_back({square, (r - 1) * 8 + (c - 1)});
        }
        if (isInsideBoard(r - 1, c + 1) &&
            isBlackPiece(board[(r - 1) * 8 + (c + 1)])) {
            moves.push_back({square, (r - 1) * 8 + (c + 1)});
        }
    } else if (piece == BLACK_PAWN) {
        // Move forward
        if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == EMPTY) {
            moves.push_back({square, (r + 1) * 8 + c});
            // Double move from starting position
            if (r == 1 && board[(r + 2) * 8 + c] == EMPTY) {
                moves.push_back({square, (r + 2) * 8 + c});
            }
        }
        // Capture diagonally
        if (isInsideBoard(r + 1, c - 1) &&
            isWhitePiece(board[(r + 1) * 8 + (c - 1)])) {
            moves.push_back({square, (r + 1) * 8 + (c - 1)});
        }
        if (isInsideBoard(r + 1, c + 1) &&
            isWhitePiece(board[(r + 1) * 8 + (c + 1)])) {
            moves.push_back({square, (r + 1) * 8 + (c + 1)});
        }
    }
}

class Game {
   public:
    vector<Piece> board;

    int colorToMove;  // 1 for white, -1 for black

    Game() {
        board.resize(64);
        initBoard(board);
        colorToMove = 1;  // White moves first
    }
};

int main() {
    while (true) {
        vector<Piece> board(64);
        string fen;
        cout << "Enter FEN string: ";
        getline(cin, fen);
        if (fen == "exit") break;

        parseFEN(fen, board);
        printBoard(board);
    }
    return 0;
}