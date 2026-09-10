#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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

enum MoveType { QUIET, DOUBLE_PUSH, CAPTURE, PROMOTION, EN_PASSANT, CASTLING };

enum CastlingRights {
    WHITE_KINGSIDE = 1 << 0,
    WHITE_QUEENSIDE = 1 << 1,
    BLACK_KINGSIDE = 1 << 2,
    BLACK_QUEENSIDE = 1 << 3
};

struct Move {
    int from;
    int to;

    MoveType type;
    Piece promotion;
    int castlingType;

    Piece movedPiece;
    Piece capturedPiece;
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

    Game() {
        board.resize(64);
        initBoard();
        colorToMove = 1;
        castlingRights =
            WHITE_KINGSIDE | WHITE_QUEENSIDE | BLACK_KINGSIDE | BLACK_QUEENSIDE;
        enPassantSquare = -1;
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

        string boardPart, activeColor, castling, enPassant, halfmoveClock,
            fullmoveNumber;

        iss >> boardPart >> activeColor >> castling >> enPassant >>
            halfmoveClock >> fullmoveNumber;

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
                    moves.push_back({square, newSq, PROMOTION, WHITE_QUEEN});
                    moves.push_back({square, newSq, PROMOTION, WHITE_ROOK});
                    moves.push_back({square, newSq, PROMOTION, WHITE_BISHOP});
                    moves.push_back({square, newSq, PROMOTION, WHITE_KNIGHT});
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
                moves.push_back({square, (r - 1) * 8 + (c - 1), CAPTURE});
            }
            if (isInsideBoard(r - 1, c + 1) &&
                isBlackPiece(board[(r - 1) * 8 + (c + 1)])) {
                moves.push_back({square, (r - 1) * 8 + (c + 1), CAPTURE});
            }
        } else if (piece == BLACK_PAWN) {
            // Move forward
            if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == EMPTY) {
                int newSq = (r + 1) * 8 + c;
                if (r == 6) {
                    moves.push_back({square, newSq, PROMOTION, BLACK_QUEEN});
                    moves.push_back({square, newSq, PROMOTION, BLACK_ROOK});
                    moves.push_back({square, newSq, PROMOTION, BLACK_BISHOP});
                    moves.push_back({square, newSq, PROMOTION, BLACK_KNIGHT});
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
                moves.push_back({square, (r + 1) * 8 + (c - 1), CAPTURE});
            }
            if (isInsideBoard(r + 1, c + 1) &&
                isWhitePiece(board[(r + 1) * 8 + (c + 1)])) {
                moves.push_back({square, (r + 1) * 8 + (c + 1), CAPTURE});
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

    void generateMoves(vector<Move>& moves) {
        generatePseudoLegalMoves(moves);
        generateCastlingMoves(moves);
        generateEnPassantMoves(moves);
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

    void makeMove(Move& move) {
        move.movedPiece = board[move.from];
        move.capturedPiece = board[move.to];

        board[move.to] = board[move.from];
        board[move.from] = EMPTY;
        colorToMove = -colorToMove;
    }

    void undoMove(Move& move) {
        board[move.from] = move.movedPiece;
        board[move.to] = move.capturedPiece;
        colorToMove = -colorToMove;
    }

    U64 perft(int depth) {
        if (depth == 0) return 1ULL;

        U64 nodes = 0;

        vector<Move> moves;
        generateMoves(moves);

        for (auto& move : moves) {
            makeMove(move);
            if (!inCheck(-colorToMove)) nodes += perft(depth - 1);
            undoMove(move);
        }

        return nodes;
    }
};

int main() {
    Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    for (int i = 1; i <= 5; i++) {
        U64 nodes = game.perft(i);
        cout << "Depth " << i << ": " << nodes << " nodes" << endl;
    }
    return 0;
}