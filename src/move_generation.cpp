#include <vector>

#include "../include/position.hpp"

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

    if (piece == (Piece::PAWN | Piece::WHITE)) {
        // Move forward
        if (isInsideBoard(r - 1, c) && board[(r - 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r - 1) * 8 + c;
            if (r == 1) {
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::QUEEN | Piece::WHITE));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::ROOK | Piece::WHITE));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::BISHOP | Piece::WHITE));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::KNIGHT | Piece::WHITE));
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
                    square, newSq, Piece::QUEEN | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::ROOK | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BISHOP | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::KNIGHT | Piece::WHITE, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
        if (isInsideBoard(r - 1, c + 1) &&
            getPieceColor(board[(r - 1) * 8 + (c + 1)]) == Color::BLACK) {
            int newSq = (r - 1) * 8 + (c + 1);
            if (r == 1) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::QUEEN | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::ROOK | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BISHOP | Piece::WHITE, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::KNIGHT | Piece::WHITE, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
    } else {
        // Move forward
        if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r + 1) * 8 + c;
            if (r == 6) {
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::QUEEN | Piece::BLACK));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::ROOK | Piece::BLACK));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::BISHOP | Piece::BLACK));
                moves.push_back(Move::quietPromotion(
                    square, newSq, Piece::KNIGHT | Piece::BLACK));
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
                    square, newSq, Piece::QUEEN | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::ROOK | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BISHOP | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::KNIGHT | Piece::BLACK, board[newSq]));
            } else {
                moves.push_back(Move::capture(square, newSq, board[newSq]));
            }
        }
        if (isInsideBoard(r + 1, c + 1) &&
            getPieceColor(board[(r + 1) * 8 + (c + 1)]) == Color::WHITE) {
            int newSq = (r + 1) * 8 + (c + 1);
            if (r == 6) {
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::QUEEN | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::ROOK | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::BISHOP | Piece::BLACK, board[newSq]));
                moves.push_back(Move::capturePromotion(
                    square, newSq, Piece::KNIGHT | Piece::BLACK, board[newSq]));
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
            board[r * 8 + (c - 1)] == (Piece::PAWN | Piece::WHITE)) {
            moves.push_back(Move::enPassant(r * 8 + (c - 1), enPassantSquare,
                                            Piece::PAWN | Piece::BLACK));
        }
        if (isInsideBoard(r, c + 1) &&
            board[r * 8 + (c + 1)] == (Piece::PAWN | Piece::WHITE)) {
            moves.push_back(Move::enPassant(r * 8 + (c + 1), enPassantSquare,
                                            Piece::PAWN | Piece::BLACK));
        }
    } else {
        if (isInsideBoard(r, c - 1) &&
            board[r * 8 + (c - 1)] == (Piece::PAWN | Piece::BLACK)) {
            moves.push_back(Move::enPassant(r * 8 + (c - 1), enPassantSquare,
                                            Piece::PAWN | Piece::WHITE));
        }
        if (isInsideBoard(r, c + 1) &&
            board[r * 8 + (c + 1)] == (Piece::PAWN | Piece::BLACK)) {
            moves.push_back(Move::enPassant(r * 8 + (c + 1), enPassantSquare,
                                            Piece::PAWN | Piece::WHITE));
        }
    }
}

void Position::generateCastlingMoves(vector<Move>& moves) {
    if (colorToMove == Color::WHITE) {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_KINGSIDE)) {
            if (board[5] == Piece::EMPTY && board[6] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::BLACK) &&
                !isSquareAttacked(5, Color::BLACK) &&
                !isSquareAttacked(6, Color::BLACK)) {
                moves.push_back(Move::castling(CastlingRights::WHITE_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_QUEENSIDE)) {
            if (board[3] == Piece::EMPTY && board[2] == Piece::EMPTY &&
                board[1] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::BLACK) &&
                !isSquareAttacked(3, Color::BLACK) &&
                !isSquareAttacked(2, Color::BLACK)) {
                moves.push_back(
                    Move::castling(CastlingRights::WHITE_QUEENSIDE));
            }
        }
    } else {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_KINGSIDE)) {
            if (board[61] == Piece::EMPTY && board[62] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::WHITE) &&
                !isSquareAttacked(61, Color::WHITE) &&
                !isSquareAttacked(62, Color::WHITE)) {
                moves.push_back(Move::castling(CastlingRights::BLACK_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_QUEENSIDE)) {
            if (board[59] == Piece::EMPTY && board[58] == Piece::EMPTY &&
                board[57] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::WHITE) &&
                !isSquareAttacked(59, Color::WHITE) &&
                !isSquareAttacked(58, Color::WHITE)) {
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

        switch (piece &
                static_cast<Piece>(0x07)) {  // Mask to get the piece type
            case Piece::PAWN:
                generatePawnMoves(square, moves);
                break;
            case Piece::KNIGHT:
                generateKnightMoves(square, moves);
                break;
            case Piece::BISHOP:
                generateBishopMoves(square, moves);
                break;
            case Piece::ROOK:
                generateRookMoves(square, moves);
                break;
            case Piece::QUEEN:
                generateQueenMoves(square, moves);
                break;
            case Piece::KING:
                generateKingMoves(square, moves);
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
