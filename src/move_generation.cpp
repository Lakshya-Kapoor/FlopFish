#include <vector>

#include "position.hpp"

using namespace std;

void Position::generateKnightMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == Piece::EMPTY) {
                moves.push_back(Move::quiet(square, newSquare));
            } else if (getPieceColor(board[newSquare]) !=
                       getPieceColor(board[square])) {
                moves.push_back(
                    Move::capture(square, newSquare, board[newSquare]));
            }
        }
    }
}

void Position::generateBishopMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;

    for (auto& dir : bishopDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == Piece::EMPTY) {
                moves.push_back(Move::quiet(square, newSquare));
            } else {
                if (getPieceColor(board[newSquare]) !=
                    getPieceColor(board[square])) {
                    moves.push_back(
                        Move::capture(square, newSquare, board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void Position::generateRookMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;

    for (auto& dir : rookDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == Piece::EMPTY) {
                moves.push_back(Move::quiet(square, newSquare));
            } else {
                if (getPieceColor(board[newSquare]) !=
                    getPieceColor(board[square])) {
                    moves.push_back(
                        Move::capture(square, newSquare, board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void Position::generateQueenMoves(int square, vector<Move>& moves) {
    generateBishopMoves(square, moves);
    generateRookMoves(square, moves);
}

void Position::generateKingMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;

    for (auto& dir : kingDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            if (board[newSquare] == Piece::EMPTY) {
                moves.push_back(Move::quiet(square, newSquare));
            } else if (getPieceColor(board[newSquare]) !=
                       getPieceColor(board[square])) {
                moves.push_back(
                    Move::capture(square, newSquare, board[newSquare]));
            }
        }
    }
}

void Position::generatePawnMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;
    Piece piece = board[square];

    if (piece == Piece::WHITE_PAWN) {
        // Move forward
        if (isInsideBoard(r - 1, c) && board[(r - 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r - 1) * 8 + c;
            if (r == 1) {
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::WHITE_QUEEN));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::WHITE_ROOK));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::WHITE_BISHOP));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::WHITE_KNIGHT));
            } else {
                moves.push_back(Move::quiet(square, newSq));
            }
            // Double move from starting position
            if (r == 6 && board[(r - 2) * 8 + c] == Piece::EMPTY) {
                moves.push_back(Move::doublePush(square, (r - 2) * 8 + c));
            }
        }

        // Capture diagonally
        if (isInsideBoard(r - 1, c - 1) &&
            getPieceColor(board[(r - 1) * 8 + (c - 1)]) == Color::BLACK) {
            int newSq = (r - 1) * 8 + (c - 1);
            if (r == 1) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_QUEEN, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_ROOK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_BISHOP, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_KNIGHT, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
        if (isInsideBoard(r - 1, c + 1) &&
            getPieceColor(board[(r - 1) * 8 + (c + 1)]) == Color::BLACK) {
            int newSq = (r - 1) * 8 + (c + 1);
            if (r == 1) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_QUEEN, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_ROOK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_BISHOP, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::WHITE_KNIGHT, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
    } else {
        // Move forward
        if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r + 1) * 8 + c;
            if (r == 6) {
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::BLACK_QUEEN));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::BLACK_ROOK));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::BLACK_BISHOP));
                moves.push_back(
                    Move::quietPromotion(square, newSq, Piece::BLACK_KNIGHT));
            } else {
                moves.push_back(Move::quiet(square, newSq));
            }
            // Double move from starting position
            if (r == 1 && board[(r + 2) * 8 + c] == Piece::EMPTY) {
                moves.push_back(Move::doublePush(square, (r + 2) * 8 + c));
            }
        }

        // Capture diagonally
        if (isInsideBoard(r + 1, c - 1) &&
            getPieceColor(board[(r + 1) * 8 + (c - 1)]) == Color::WHITE) {
            int newSq = (r + 1) * 8 + (c - 1);
            if (r == 6) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_QUEEN, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_ROOK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_BISHOP, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_KNIGHT, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
        if (isInsideBoard(r + 1, c + 1) &&
            getPieceColor(board[(r + 1) * 8 + (c + 1)]) == Color::WHITE) {
            int newSq = (r + 1) * 8 + (c + 1);
            if (r == 6) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_QUEEN, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_ROOK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_BISHOP, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BLACK_KNIGHT, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
    }
}

void Position::generateEnPassantMoves(vector<Move>& moves) {
    if (enPassantSquare == -1) return;

    int r = enPassantSquare / 8, c = enPassantSquare % 8;
    if (colorToMove == Color::WHITE) {
        if (isInsideBoard(r, c - 1) &&
            board[r * 8 + (c - 1)] == Piece::WHITE_PAWN) {
            moves.push_back(Move::enPassant(r * 8 + (c - 1), (r - 1) * 8 + c,
                                            Piece::BLACK_PAWN));
        }
        if (isInsideBoard(r, c + 1) &&
            board[r * 8 + (c + 1)] == Piece::WHITE_PAWN) {
            moves.push_back(Move::enPassant(r * 8 + (c + 1), (r - 1) * 8 + c,
                                            Piece::BLACK_PAWN));
        }
    } else {
        if (isInsideBoard(r, c - 1) &&
            board[r * 8 + (c - 1)] == Piece::BLACK_PAWN) {
            moves.push_back(Move::enPassant(r * 8 + (c - 1), (r + 1) * 8 + c,
                                            Piece::WHITE_PAWN));
        }
        if (isInsideBoard(r, c + 1) &&
            board[r * 8 + (c + 1)] == Piece::BLACK_PAWN) {
            moves.push_back(Move::enPassant(r * 8 + (c + 1), (r + 1) * 8 + c,
                                            Piece::WHITE_PAWN));
        }
    }
}

void Position::generateCastlingMoves(vector<Move>& moves) {
    if (colorToMove == Color::WHITE) {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_KINGSIDE)) {
            if (board[61] == Piece::EMPTY && board[62] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::BLACK) &&
                !isSquareAttacked(61, Color::BLACK) &&
                !isSquareAttacked(62, Color::BLACK)) {
                moves.push_back(Move::castling(CastlingRights::WHITE_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_QUEENSIDE)) {
            if (board[59] == Piece::EMPTY && board[58] == Piece::EMPTY &&
                board[57] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::BLACK) &&
                !isSquareAttacked(59, Color::BLACK) &&
                !isSquareAttacked(58, Color::BLACK)) {
                moves.push_back(
                    Move::castling(CastlingRights::WHITE_QUEENSIDE));
            }
        }
    } else {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_KINGSIDE)) {
            if (board[5] == Piece::EMPTY && board[6] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::WHITE) &&
                !isSquareAttacked(5, Color::WHITE) &&
                !isSquareAttacked(6, Color::WHITE)) {
                moves.push_back(Move::castling(CastlingRights::BLACK_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_QUEENSIDE)) {
            if (board[3] == Piece::EMPTY && board[2] == Piece::EMPTY &&
                board[1] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::WHITE) &&
                !isSquareAttacked(3, Color::WHITE) &&
                !isSquareAttacked(2, Color::WHITE)) {
                moves.push_back(
                    Move::castling(CastlingRights::BLACK_QUEENSIDE));
            }
        }
    }
}

vector<Move> Position::generatePseudoLegalMoves() {
    vector<Move> moves;

    generateCastlingMoves(moves);
    generateEnPassantMoves(moves);

    for (int square = 0; square < 64; square++) {
        Piece piece = board[square];
        if (piece == Piece::EMPTY) continue;

        if (getPieceColor(piece) != colorToMove) continue;

        switch (piece) {
            case Piece::WHITE_PAWN:
            case Piece::BLACK_PAWN:
                generatePawnMoves(square, moves);
                break;
            case Piece::WHITE_KNIGHT:
            case Piece::BLACK_KNIGHT:
                generateKnightMoves(square, moves);
                break;
            case Piece::WHITE_BISHOP:
            case Piece::BLACK_BISHOP:
                generateBishopMoves(square, moves);
                break;
            case Piece::WHITE_ROOK:
            case Piece::BLACK_ROOK:
                generateRookMoves(square, moves);
                break;
            case Piece::WHITE_QUEEN:
            case Piece::BLACK_QUEEN:
                generateQueenMoves(square, moves);
                break;
            case Piece::WHITE_KING:
            case Piece::BLACK_KING:
                generateKingMoves(square, moves);
                break;
            case Piece::EMPTY:
                break;
        }
    }
    return moves;
}

vector<Move> Position::generateLegalMoves() {
    vector<Move> pseudoMoves = generatePseudoLegalMoves();
    vector<Move> legalMoves;

    StateInfo st;
    for (const auto& move : pseudoMoves) {
        makeMove(move, st);
        if (!inCheck(-colorToMove)) {
            legalMoves.push_back(move);
        }
        undoMove(move, st);
    }

    return legalMoves;
}
