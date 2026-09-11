#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long U64;

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

enum MoveType {
    QUIET,
    DOUBLE_PUSH,
    CAPTURE,
    QUIET_PROMOTION,
    CAPTURE_PROMOTION,
    EN_PASSANT,
    CASTLING
};

enum CastlingRights {
    WHITE_KINGSIDE = 1 << 0,
    WHITE_QUEENSIDE = 1 << 1,
    BLACK_KINGSIDE = 1 << 2,
    BLACK_QUEENSIDE = 1 << 3
};

enum GameState { ONGOING, CHECKMATE, STALEMATE, DRAW };

struct Move {
    int from;
    int to;

    MoveType type;
    Piece promotionPiece;
    int castlingType;
    Piece capturedPiece;

    int oldCastlingRights;
    int oldEnPassantSquare;
    int oldHalfmoveClock;
    int oldFullmoveNumber;
};

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

class Game {
   public:
    vector<Piece> board;
    int colorToMove;  // 1 for white, -1 for black
    int castlingRights;
    int enPassantSquare;  // -1 if no en passant square
    int halfmoveClock;
    int fullmoveNumber;

    const int pawnTable[64] = {
        0,  0,  0,  0,   0,   0,  0,  0,  50, 50, 50,  50, 50, 50,  50, 50,
        10, 10, 20, 30,  30,  20, 10, 10, 5,  5,  10,  25, 25, 10,  5,  5,
        0,  0,  0,  20,  20,  0,  0,  0,  5,  -5, -10, 0,  0,  -10, -5, 5,
        5,  10, 10, -20, -20, 10, 10, 5,  0,  0,  0,   0,  0,  0,   0,  0};

    const int knightTable[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50, -40, -20, 0,   5,   5,
        0,   -20, -40, -30, 5,   10,  15,  15,  10,  5,   -30, -30, 0,
        15,  20,  20,  15,  0,   -30, -30, 5,   15,  20,  20,  15,  5,
        -30, -30, 0,   10,  15,  15,  10,  0,   -30, -40, -20, 0,   0,
        0,   0,   -20, -40, -50, -40, -30, -30, -30, -30, -40, -50};

    const int bishopTable[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20, -10, 5,   0,   0,   0,
        0,   5,   -10, -10, 10,  10,  10,  10,  10,  10,  -10, -10, 0,
        10,  10,  10,  10,  0,   -10, -10, 5,   5,   10,  10,  5,   5,
        -10, -10, 0,   5,   10,  10,  5,   0,   -10, -10, 0,   0,   0,
        0,   0,   0,   -10, -20, -10, -10, -10, -10, -10, -10, -20};

    const int rookTable[64] = {0,  0,  0,  5,  5, 0,  0,  0, -5, 0, 0,  0,  0,
                               0,  0,  -5, -5, 0, 0,  0,  0, 0,  0, -5, -5, 0,
                               0,  0,  0,  0,  0, -5, -5, 0, 0,  0, 0,  0,  0,
                               -5, -5, 0,  0,  0, 0,  0,  0, -5, 5, 10, 10, 10,
                               10, 10, 10, 5,  0, 0,  0,  0, 0,  0, 0,  0};

    const int queenTable[64] = {
        -20, -10, -10, -5,  -5,  -10, -10, -20, -10, 0,   0,   0,  0,
        0,   0,   -10, -10, 0,   5,   5,   5,   5,   0,   -10, -5, 0,
        5,   5,   5,   5,   0,   -5,  0,   0,   5,   5,   5,   5,  0,
        -5,  -10, 5,   5,   5,   5,   5,   5,   -10, -10, 0,   5,  0,
        0,   5,   0,   -10, -20, -10, -10, -5,  -5,  -10, -10, -20};

    const int kingTable[64] = {
        -30, -40, -40, -50, -50, -40, -40, -30, -30, -40, -40, -50, -50,
        -40, -40, -30, -30, -40, -40, -50, -50, -40, -40, -30, -30, -40,
        -40, -50, -50, -40, -40, -30, -20, -30, -30, -40, -40, -30, -30,
        -20, -10, -20, -20, -20, -20, -20, -20, -10, 20,  20,  0,   0,
        0,   0,   20,  20,  20,  30,  10,  0,   0,   10,  30,  20};

    Game() {
        board.resize(64);
        initBoard();
        colorToMove = 1;
        castlingRights =
            WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE;
        enPassantSquare = -1;
        halfmoveClock = 0;
        fullmoveNumber = 1;
    }

    Game(string fen) {
        board.resize(64);
        parseFEN(fen);
    }

    void initBoard() {
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

    void printBoard() {
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

    void parseFEN(string fen) {
        istringstream iss(fen);

        string boardPart, activeColor, castling, enPassant, halfClock,
            fullNumber;

        iss >> boardPart >> activeColor >> castling >> enPassant >> halfClock >>
            fullNumber;

        int index = 0;
        for (char c : boardPart) {
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

        colorToMove = (activeColor == "w") ? 1 : -1;

        castlingRights = 0;
        for (char c : castling) {
            if (c == 'K')
                castlingRights |= WHITE_KINGSIDE;
            else if (c == 'Q')
                castlingRights |= WHITE_QUEENSIDE;
            else if (c == 'k')
                castlingRights |= BLACK_KINGSIDE;
            else if (c == 'q')
                castlingRights |= BLACK_QUEENSIDE;
        }

        if (enPassant == "-")
            enPassantSquare = -1;
        else {
            int file = enPassant[0] - 'a';
            int rank = 8 - (enPassant[1] - '0');
            enPassantSquare = rank * 8 + file;
        }

        halfmoveClock = stoi(halfClock);
        fullmoveNumber = stoi(fullNumber);
    }

    void generateKnightMoves(int square, vector<Move>& moves) {
        int knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                 {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

        int r = square / 8, c = square % 8;
        for (auto& move : knightMoves) {
            int newR = r + move[0], newC = c + move[1];
            if (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (pieceColor(board[newSquare]) == EMPTY)
                    moves.push_back({square, newSquare, QUIET});
                else if (pieceColor(board[newSquare]) !=
                         pieceColor(board[square]))
                    moves.push_back({square, newSquare, CAPTURE});
            }
        }
    }

    void generateBishopMoves(int square, vector<Move>& moves) {
        int directions[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        int r = square / 8, c = square % 8;

        for (auto& dir : directions) {
            int newR = r + dir[0], newC = c + dir[1];
            while (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (board[newSquare] == EMPTY) {
                    moves.push_back({square, newSquare, QUIET});
                } else {
                    if (pieceColor(board[newSquare]) !=
                        pieceColor(board[square])) {
                        moves.push_back({square, newSquare, CAPTURE});
                    }
                    break;  // Stop if we hit a piece
                }
                newR += dir[0];
                newC += dir[1];
            }
        }
    }

    void generateRookMoves(int square, vector<Move>& moves) {
        int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int r = square / 8, c = square % 8;

        for (auto& dir : directions) {
            int newR = r + dir[0], newC = c + dir[1];
            while (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (board[newSquare] == EMPTY) {
                    moves.push_back({square, newSquare, QUIET});
                } else {
                    if (pieceColor(board[newSquare]) !=
                        pieceColor(board[square])) {
                        moves.push_back({square, newSquare, CAPTURE});
                    }
                    break;  // Stop if we hit a piece
                }
                newR += dir[0];
                newC += dir[1];
            }
        }
    }

    void generateQueenMoves(int square, vector<Move>& moves) {
        generateBishopMoves(square, moves);
        generateRookMoves(square, moves);
    }

    void generateKingMoves(int square, vector<Move>& moves) {
        int kingMoves[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

        int r = square / 8, c = square % 8;
        for (auto& move : kingMoves) {
            int newR = r + move[0], newC = c + move[1];
            if (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (pieceColor(board[newSquare]) == EMPTY)
                    moves.push_back({square, newSquare, QUIET});
                else if (pieceColor(board[newSquare]) !=
                         pieceColor(board[square]))
                    moves.push_back({square, newSquare, CAPTURE});
            }
        }
    }

    void generatePawnMoves(int square, vector<Move>& moves) {
        int r = square / 8, c = square % 8;
        Piece piece = board[square];

        if (piece == WHITE_PAWN) {
            // Move forward
            if (isInsideBoard(r - 1, c) && board[(r - 1) * 8 + c] == EMPTY) {
                int newSq = (r - 1) * 8 + c;
                if (r == 1) {
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, WHITE_QUEEN});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, WHITE_ROOK});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, WHITE_BISHOP});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, WHITE_KNIGHT});
                } else {
                    moves.push_back({square, newSq, QUIET});
                }
                // Double move from starting position
                if (r == 6 && board[(r - 2) * 8 + c] == EMPTY) {
                    moves.push_back({square, (r - 2) * 8 + c, DOUBLE_PUSH});
                }
            }
            // Capture diagonally
            if (isInsideBoard(r - 1, c - 1) &&
                isBlackPiece(board[(r - 1) * 8 + (c - 1)])) {
                int newSq = (r - 1) * 8 + (c - 1);
                if (r == 1) {
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_QUEEN});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_ROOK});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_BISHOP});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_KNIGHT});
                } else {
                    moves.push_back({square, newSq, CAPTURE});
                }
            }
            if (isInsideBoard(r - 1, c + 1) &&
                isBlackPiece(board[(r - 1) * 8 + (c + 1)])) {
                int newSq = (r - 1) * 8 + (c + 1);
                if (r == 1) {
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_QUEEN});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_ROOK});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_BISHOP});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, WHITE_KNIGHT});
                } else {
                    moves.push_back({square, newSq, CAPTURE});
                }
            }
        } else if (piece == BLACK_PAWN) {
            // Move forward
            if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == EMPTY) {
                int newSq = (r + 1) * 8 + c;
                if (r == 6) {
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, BLACK_QUEEN});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, BLACK_ROOK});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, BLACK_BISHOP});
                    moves.push_back(
                        {square, newSq, QUIET_PROMOTION, BLACK_KNIGHT});
                } else {
                    moves.push_back({square, newSq, QUIET});
                }
                // Double move from starting position
                if (r == 1 && board[(r + 2) * 8 + c] == EMPTY) {
                    moves.push_back({square, (r + 2) * 8 + c, DOUBLE_PUSH});
                }
            }
            // Capture diagonally
            if (isInsideBoard(r + 1, c - 1) &&
                isWhitePiece(board[(r + 1) * 8 + (c - 1)])) {
                int newSq = (r + 1) * 8 + (c - 1);
                if (r == 6) {
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_QUEEN});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_ROOK});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_BISHOP});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_KNIGHT});
                } else {
                    moves.push_back({square, newSq, CAPTURE});
                }
            }
            if (isInsideBoard(r + 1, c + 1) &&
                isWhitePiece(board[(r + 1) * 8 + (c + 1)])) {
                int newSq = (r + 1) * 8 + (c + 1);
                if (r == 6) {
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_QUEEN});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_ROOK});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_BISHOP});
                    moves.push_back(
                        {square, newSq, CAPTURE_PROMOTION, BLACK_KNIGHT});
                } else {
                    moves.push_back({square, newSq, CAPTURE});
                }
            }
        }
    }

    void generateEnPassantMoves(vector<Move>& moves) {
        if (enPassantSquare == -1) return;

        int r = enPassantSquare / 8, c = enPassantSquare % 8;
        if (colorToMove == 1) {
            if (isInsideBoard(r, c - 1) &&
                board[r * 8 + (c - 1)] == WHITE_PAWN) {
                moves.push_back({r * 8 + (c - 1), (r - 1) * 8 + c, EN_PASSANT});
            }
            if (isInsideBoard(r, c + 1) &&
                board[r * 8 + (c + 1)] == WHITE_PAWN) {
                moves.push_back({r * 8 + (c + 1), (r - 1) * 8 + c, EN_PASSANT});
            }
        } else {
            if (isInsideBoard(r, c - 1) &&
                board[r * 8 + (c - 1)] == BLACK_PAWN) {
                moves.push_back({r * 8 + (c - 1), (r + 1) * 8 + c, EN_PASSANT});
            }
            if (isInsideBoard(r, c + 1) &&
                board[r * 8 + (c + 1)] == BLACK_PAWN) {
                moves.push_back({r * 8 + (c + 1), (r + 1) * 8 + c, EN_PASSANT});
            }
        }
    }

    void generateCastlingMoves(vector<Move>& moves) {
        Move move;
        move.type = CASTLING;
        if (colorToMove == 1) {
            if (castlingRights & WHITE_KINGSIDE) {
                if (board[61] == EMPTY && board[62] == EMPTY &&
                    !isSquareAttacked(60, -1) && !isSquareAttacked(61, -1) &&
                    !isSquareAttacked(62, -1)) {
                    move.castlingType = WHITE_KINGSIDE;
                    moves.push_back(move);
                }
            }

            if (castlingRights & WHITE_QUEENSIDE) {
                if (board[59] == EMPTY && board[58] == EMPTY &&
                    board[57] == EMPTY && !isSquareAttacked(60, -1) &&
                    !isSquareAttacked(59, -1) && !isSquareAttacked(58, -1)) {
                    move.castlingType = WHITE_QUEENSIDE;
                    moves.push_back(move);
                }
            }
        } else {
            if (castlingRights & BLACK_KINGSIDE) {
                if (board[5] == EMPTY && board[6] == EMPTY &&
                    !isSquareAttacked(4, 1) && !isSquareAttacked(5, 1) &&
                    !isSquareAttacked(6, 1)) {
                    move.castlingType = BLACK_KINGSIDE;
                    moves.push_back(move);
                }
            }

            if (castlingRights & BLACK_QUEENSIDE) {
                if (board[3] == EMPTY && board[2] == EMPTY &&
                    board[1] == EMPTY && !isSquareAttacked(4, 1) &&
                    !isSquareAttacked(3, 1) && !isSquareAttacked(2, 1)) {
                    move.castlingType = BLACK_QUEENSIDE;
                    moves.push_back(move);
                }
            }
        }
    }

    void generatePseudoLegalMoves(vector<Move>& moves) {
        generateCastlingMoves(moves);
        generateEnPassantMoves(moves);
        if (colorToMove == 1) {
            for (int i = 0; i < 64; i++) {
                if (isWhitePiece(board[i])) {
                    if (board[i] == WHITE_PAWN)
                        generatePawnMoves(i, moves);
                    else if (board[i] == WHITE_KNIGHT)
                        generateKnightMoves(i, moves);
                    else if (board[i] == WHITE_BISHOP)
                        generateBishopMoves(i, moves);
                    else if (board[i] == WHITE_ROOK)
                        generateRookMoves(i, moves);
                    else if (board[i] == WHITE_QUEEN)
                        generateQueenMoves(i, moves);
                    else if (board[i] == WHITE_KING)
                        generateKingMoves(i, moves);
                }
            }
        } else {
            for (int i = 0; i < 64; i++) {
                if (isBlackPiece(board[i])) {
                    if (board[i] == BLACK_PAWN)
                        generatePawnMoves(i, moves);
                    else if (board[i] == BLACK_KNIGHT)
                        generateKnightMoves(i, moves);
                    else if (board[i] == BLACK_BISHOP)
                        generateBishopMoves(i, moves);
                    else if (board[i] == BLACK_ROOK)
                        generateRookMoves(i, moves);
                    else if (board[i] == BLACK_QUEEN)
                        generateQueenMoves(i, moves);
                    else if (board[i] == BLACK_KING)
                        generateKingMoves(i, moves);
                }
            }
        }
    }

    // generate all moves such that the king is not in check after the move
    void generateLegalMoves(vector<Move>& moves) {
        vector<Move> pseudoMoves;
        generatePseudoLegalMoves(pseudoMoves);

        for (auto& move : pseudoMoves) {
            makeMove(move);
            if (!inCheck(-colorToMove)) moves.push_back(move);
            undoMove(move);
        }
    }

    // Check if the king of the given color is in check
    bool inCheck(int color) {
        int kingSquare = -1;
        for (int i = 0; i < 64; i++) {
            if ((color == 1 && board[i] == WHITE_KING) ||
                (color == -1 && board[i] == BLACK_KING)) {
                kingSquare = i;
                break;
            }
        }

        return isSquareAttacked(kingSquare, -color);
    }

    // Check if a square is attacked by a piece of the given color
    bool isSquareAttacked(int square, int byColor) {
        int r = square / 8, c = square % 8;

        // knight attacking
        int knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                 {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
        for (auto& move : knightMoves) {
            int newR = r + move[0], newC = c + move[1];
            if (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (byColor == 1 && board[newSquare] == WHITE_KNIGHT)
                    return true;
                if (byColor == -1 && board[newSquare] == BLACK_KNIGHT)
                    return true;
            }
        }

        // diagonal attack by bishop or queen
        int bishopDirections[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& dir : bishopDirections) {
            int newR = r + dir[0], newC = c + dir[1];
            while (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (board[newSquare] != EMPTY) {
                    if (byColor == 1 && (board[newSquare] == WHITE_BISHOP ||
                                         board[newSquare] == WHITE_QUEEN))
                        return true;
                    if (byColor == -1 && (board[newSquare] == BLACK_BISHOP ||
                                          board[newSquare] == BLACK_QUEEN))
                        return true;
                    break;
                }

                newR += dir[0];
                newC += dir[1];
            }
        }

        // straight attack by rook or queen
        int rookDirections[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (auto& dir : rookDirections) {
            int newR = r + dir[0], newC = c + dir[1];
            while (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (board[newSquare] != EMPTY) {
                    if (byColor == 1 && (board[newSquare] == WHITE_ROOK ||
                                         board[newSquare] == WHITE_QUEEN))
                        return true;
                    if (byColor == -1 && (board[newSquare] == BLACK_ROOK ||
                                          board[newSquare] == BLACK_QUEEN))
                        return true;
                    break;
                }
                newR += dir[0];
                newC += dir[1];
            }
        }

        // king attack
        int kingMoves[8][2] = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                               {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
        for (auto& move : kingMoves) {
            int newR = r + move[0], newC = c + move[1];
            if (isInsideBoard(newR, newC)) {
                int newSquare = newR * 8 + newC;
                if (byColor == 1 && board[newSquare] == WHITE_KING) return true;
                if (byColor == -1 && board[newSquare] == BLACK_KING)
                    return true;
            }
        }

        // pawn attack
        if (byColor == 1) {
            if (isInsideBoard(r + 1, c - 1) &&
                board[(r + 1) * 8 + (c - 1)] == WHITE_PAWN)
                return true;
            if (isInsideBoard(r + 1, c + 1) &&
                board[(r + 1) * 8 + (c + 1)] == WHITE_PAWN)
                return true;
        } else {
            if (isInsideBoard(r - 1, c - 1) &&
                board[(r - 1) * 8 + (c - 1)] == BLACK_PAWN)
                return true;
            if (isInsideBoard(r - 1, c + 1) &&
                board[(r - 1) * 8 + (c + 1)] == BLACK_PAWN)
                return true;
        }

        return false;
    }

    void removeCastlingRights(Move& move) {
        if (castlingRights & WHITE_KINGSIDE &&
            (move.from == 63 || move.from == 60 || move.to == 63)) {
            castlingRights &= ~WHITE_KINGSIDE;
        }
        if (castlingRights & WHITE_QUEENSIDE &&
            (move.from == 56 || move.from == 60 || move.to == 56)) {
            castlingRights &= ~WHITE_QUEENSIDE;
        }
        if (castlingRights & BLACK_KINGSIDE &&
            (move.from == 7 || move.from == 4 || move.to == 7)) {
            castlingRights &= ~BLACK_KINGSIDE;
        }
        if (castlingRights & BLACK_QUEENSIDE &&
            (move.from == 0 || move.from == 4 || move.to == 0)) {
            castlingRights &= ~BLACK_QUEENSIDE;
        }
    }

    void makeMove(Move& move) {
        move.oldCastlingRights = castlingRights;
        move.oldEnPassantSquare = enPassantSquare;
        move.oldHalfmoveClock = halfmoveClock;
        move.oldFullmoveNumber = fullmoveNumber;

        if (move.type == QUIET) {
            removeCastlingRights(move);
            board[move.to] = board[move.from];
            board[move.from] = EMPTY;

        } else if (move.type == DOUBLE_PUSH) {
            enPassantSquare = move.to;
            board[move.to] = board[move.from];
            board[move.from] = EMPTY;

        } else if (move.type == CAPTURE) {
            removeCastlingRights(move);
            move.capturedPiece = board[move.to];
            board[move.to] = board[move.from];
            board[move.from] = EMPTY;
        } else if (move.type == QUIET_PROMOTION) {
            board[move.to] = move.promotionPiece;
            board[move.from] = EMPTY;
        } else if (move.type == CAPTURE_PROMOTION) {
            move.capturedPiece = board[move.to];
            board[move.to] = move.promotionPiece;
            board[move.from] = EMPTY;
        } else if (move.type == EN_PASSANT) {
            move.capturedPiece = board[enPassantSquare];
            board[enPassantSquare] = EMPTY;
            board[move.to] = board[move.from];
            board[move.from] = EMPTY;
        } else if (move.type & CASTLING) {
            if (move.castlingType == WHITE_KINGSIDE) {
                board[60] = EMPTY;
                board[61] = WHITE_ROOK;
                board[62] = WHITE_KING;
                board[63] = EMPTY;
                castlingRights &= ~(WHITE_KINGSIDE | WHITE_QUEENSIDE);
            } else if (move.castlingType == WHITE_QUEENSIDE) {
                board[60] = EMPTY;
                board[59] = WHITE_ROOK;
                board[58] = WHITE_KING;
                board[56] = EMPTY;
                castlingRights &= ~(WHITE_KINGSIDE | WHITE_QUEENSIDE);
            } else if (move.castlingType == BLACK_KINGSIDE) {
                board[4] = EMPTY;
                board[5] = BLACK_ROOK;
                board[6] = BLACK_KING;
                board[7] = EMPTY;
                castlingRights &= ~(BLACK_KINGSIDE | BLACK_QUEENSIDE);
            } else if (move.castlingType == BLACK_QUEENSIDE) {
                board[4] = EMPTY;
                board[3] = BLACK_ROOK;
                board[2] = BLACK_KING;
                board[0] = EMPTY;
                castlingRights &= ~(BLACK_KINGSIDE | BLACK_QUEENSIDE);
            }
        }

        if (move.type != DOUBLE_PUSH) enPassantSquare = -1;

        if (move.type != CASTLING &&
            (board[move.from] == WHITE_PAWN || board[move.from] == BLACK_PAWN ||
             move.type == CAPTURE))
            halfmoveClock = 0;
        else
            halfmoveClock++;

        if (colorToMove == -1) fullmoveNumber++;

        colorToMove = -colorToMove;
    }

    void undoMove(Move& move) {
        castlingRights = move.oldCastlingRights;
        enPassantSquare = move.oldEnPassantSquare;
        halfmoveClock = move.oldHalfmoveClock;
        fullmoveNumber = move.oldFullmoveNumber;

        if (move.type == QUIET) {
            board[move.from] = board[move.to];
            board[move.to] = EMPTY;

        } else if (move.type == DOUBLE_PUSH) {
            board[move.from] = board[move.to];
            board[move.to] = EMPTY;

        } else if (move.type == CAPTURE) {
            board[move.from] = board[move.to];
            board[move.to] = move.capturedPiece;

        } else if (move.type == QUIET_PROMOTION) {
            board[move.from] = (isWhitePiece(move.promotionPiece) == 1)
                                   ? WHITE_PAWN
                                   : BLACK_PAWN;
            board[move.to] = EMPTY;
        } else if (move.type == CAPTURE_PROMOTION) {
            board[move.from] = (isWhitePiece(move.promotionPiece) == 1)
                                   ? WHITE_PAWN
                                   : BLACK_PAWN;
            board[move.to] = move.capturedPiece;

        } else if (move.type == EN_PASSANT) {
            board[move.from] = board[move.to];
            board[move.to] = EMPTY;
            board[move.oldEnPassantSquare] = move.capturedPiece;

        } else if (move.type & CASTLING) {
            if (move.castlingType == WHITE_KINGSIDE) {
                board[60] = WHITE_KING;
                board[61] = EMPTY;
                board[62] = EMPTY;
                board[63] = WHITE_ROOK;

            } else if (move.castlingType == WHITE_QUEENSIDE) {
                board[60] = WHITE_KING;
                board[59] = EMPTY;
                board[58] = EMPTY;
                board[56] = WHITE_ROOK;

            } else if (move.castlingType == BLACK_KINGSIDE) {
                board[4] = BLACK_KING;
                board[5] = EMPTY;
                board[6] = EMPTY;
                board[7] = BLACK_ROOK;

            } else if (move.castlingType == BLACK_QUEENSIDE) {
                board[4] = BLACK_KING;
                board[3] = EMPTY;
                board[2] = EMPTY;
                board[0] = BLACK_ROOK;
            }
        }

        colorToMove = -colorToMove;
    }

    U64 perft(int depth) {
        if (depth == 0) return 1ULL;

        U64 nodes = 0;

        vector<Move> moves;
        generatePseudoLegalMoves(moves);

        for (auto& move : moves) {
            makeMove(move);
            if (!inCheck(-colorToMove)) nodes += perft(depth - 1);
            undoMove(move);
        }

        return nodes;
    }

    int evaluateMaterial() {
        int score = 0;
        for (Piece& piece : board) {
            switch (piece) {
                case WHITE_PAWN:
                    score += 100;
                    break;
                case WHITE_KNIGHT:
                    score += 320;
                    break;
                case WHITE_BISHOP:
                    score += 330;
                    break;
                case WHITE_ROOK:
                    score += 500;
                    break;
                case WHITE_QUEEN:
                    score += 900;
                    break;
                case BLACK_PAWN:
                    score -= 100;
                    break;
                case BLACK_KNIGHT:
                    score -= 320;
                    break;
                case BLACK_BISHOP:
                    score -= 330;
                    break;
                case BLACK_ROOK:
                    score -= 500;
                    break;
                case BLACK_QUEEN:
                    score -= 900;
                    break;
                default:
                    break;
            }
        }
        return score;
    }

    int evaluatePieceSquareTables() {
        int score = 0;
        for (int i = 0; i < 64; i++) {
            Piece piece = board[i];
            if (piece == WHITE_PAWN)
                score += pawnTable[i];
            else if (piece == WHITE_KNIGHT)
                score += knightTable[i];
            else if (piece == WHITE_BISHOP)
                score += bishopTable[i];
            else if (piece == WHITE_ROOK)
                score += rookTable[i];
            else if (piece == WHITE_QUEEN)
                score += queenTable[i];
            else if (piece == BLACK_PAWN)
                score -= pawnTable[63 - i];
            else if (piece == BLACK_KNIGHT)
                score -= knightTable[63 - i];
            else if (piece == BLACK_BISHOP)
                score -= bishopTable[63 - i];
            else if (piece == BLACK_ROOK)
                score -= rookTable[63 - i];
            else if (piece == BLACK_QUEEN)
                score -= queenTable[63 - i];
        }
        return score;
    }

    int evaluateMobility() {
        vector<Move> whiteMoves;
        vector<Move> blackMoves;

        if (colorToMove == 1) {
            generatePseudoLegalMoves(whiteMoves);
            colorToMove = -1;
            generatePseudoLegalMoves(blackMoves);
            colorToMove = 1;
        } else {
            generatePseudoLegalMoves(blackMoves);
            colorToMove = 1;
            generatePseudoLegalMoves(whiteMoves);
            colorToMove = -1;
        }

        int score = (whiteMoves.size() - blackMoves.size());
        return score;
    }

    int evaluate() {
        vector<Move> moves;
        generateLegalMoves(moves);

        GameState state = getGameState(moves);
        if (state == CHECKMATE) {
            return -colorToMove * 100000;  // Checkmate
        } else if (state == STALEMATE || state == DRAW) {
            return 0;
        }

        int score = 0;
        score += evaluateMaterial();
        score += evaluateMobility();
        score += evaluatePieceSquareTables();
        return score;
    }

    GameState getGameState(vector<Move>& moves) {
        if (moves.empty()) {
            if (inCheck(colorToMove))
                return CHECKMATE;
            else
                return STALEMATE;
        }

        if (halfmoveClock >= 100) return DRAW;

        return ONGOING;
    }

    Move findBestMove(int depth) {
        vector<Move> moves;
        generateLegalMoves(moves);

        Move bestMove;
        int evalColor = colorToMove;
        int maxScore = INT_MIN;

        for (Move& move : moves) {
            makeMove(move);

            int score = -negamax(depth - 1);
            if (score > maxScore) {
                maxScore = score;
                bestMove = move;
            }

            undoMove(move);
        }

        return bestMove;
    }

    int negamax(int depth) {
        if (depth == 0) return colorToMove * evaluate();

        int maxScore = INT_MIN;

        vector<Move> moves;
        generateLegalMoves(moves);

        GameState state = getGameState(moves);
        if (state == CHECKMATE) return -100000;
        if (state == STALEMATE || state == DRAW) return 0;

        for (Move& move : moves) {
            makeMove(move);

            int score = -negamax(depth - 1);
            maxScore = max(maxScore, score);

            undoMove(move);
        }

        return maxScore;
    }

    void gameLoop() {
        int cnt = 1;
        printBoard();
        while (true) {
            vector<Move> moves;
            generateLegalMoves(moves);
            GameState state = getGameState(moves);
            if (state == CHECKMATE) {
                cout << (colorToMove == 1 ? "Black" : "White")
                     << " wins by checkmate!" << endl;
                break;
            } else if (state == STALEMATE) {
                cout << "Game ends in stalemate!" << endl;
                break;
            } else if (state == DRAW) {
                cout << "Game ends in a draw!" << endl;
                break;
            }

            Move move;
            if (colorToMove == 1) {
                move = findBestMove(4);
            } else {
                move = findBestMove(4);
            }

            makeMove(move);
            printBoard();
            cout << cnt++ << endl;
        }
    }
};

int main() {
    Game game;
    game.gameLoop();

    // Game game(
    //     "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -
    //     0 " "10");
    // for (int depth = 1; depth <= 6; depth++) {
    //     U64 nodes = game.perft(depth);
    //     cout << "Depth: " << depth << ", Nodes: " << nodes << endl;
    // }

    return 0;
}