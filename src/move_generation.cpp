#include <vector>

#include "position.hpp"

using namespace std;

void Position::generateKnightMoves(int square, vector<Move>& moves,
                                   const LegalityInfo& info) {
    int r = square / 8, c = square % 8;

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (info.legalityRespected(square, newSquare)) {
                if (board[newSquare] == Piece::EMPTY) {
                    moves.push_back(
                        Move::quiet(square, newSquare, board[square]));
                } else if (getPieceColor(board[newSquare]) !=
                           getPieceColor(board[square])) {
                    moves.push_back(Move::capture(
                        square, newSquare, board[square], board[newSquare]));
                }
            };
        }
    }
}

void Position::generateBishopMoves(int square, vector<Move>& moves,
                                   const LegalityInfo& info) {
    int r = square / 8, c = square % 8;

    for (auto& dir : bishopDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                if (info.legalityRespected(square, newSquare)) {
                    moves.push_back(
                        Move::quiet(square, newSquare, board[square]));
                }
            } else {
                if (info.legalityRespected(square, newSquare) &&
                    getPieceColor(board[newSquare]) !=
                        getPieceColor(board[square])) {
                    moves.push_back(Move::capture(
                        square, newSquare, board[square], board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }

            newR += dir[0];
            newC += dir[1];
        }
    }
}

void Position::generateRookMoves(int square, vector<Move>& moves,
                                 const LegalityInfo& info) {
    int r = square / 8, c = square % 8;

    for (auto& dir : rookDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                if (info.legalityRespected(square, newSquare)) {
                    moves.push_back(
                        Move::quiet(square, newSquare, board[square]));
                }
            } else {
                if (info.legalityRespected(square, newSquare) &&
                    getPieceColor(board[newSquare]) !=
                        getPieceColor(board[square])) {
                    moves.push_back(Move::capture(
                        square, newSquare, board[square], board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void Position::generateQueenMoves(int square, vector<Move>& moves,
                                  const LegalityInfo& info) {
    generateBishopMoves(square, moves, info);
    generateRookMoves(square, moves, info);
}

void Position::generateKingMoves(int square, vector<Move>& moves) {
    int r = square / 8, c = square % 8;

    for (auto& dir : kingDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            Move move;
            if (board[newSquare] == Piece::EMPTY) {
                move = Move::quiet(square, newSquare, board[square]);
            } else if (getPieceColor(board[newSquare]) !=
                       getPieceColor(board[square])) {
                move = Move::capture(square, newSquare, board[square],
                                     board[newSquare]);
            } else {
                continue;
            }

            StateInfo savedState;
            makeMove(move, savedState);
            bool legal = !inCheck(-colorToMove);
            undoMove(move, savedState);

            if (legal) moves.push_back(move);
        }
    }
}

void Position::generatePawnMoves(int square, vector<Move>& moves,
                                 const LegalityInfo& info) {
    int r = square / 8, c = square % 8;
    Piece piece = board[square];

    if (piece == Piece::WHITE_PAWN) {
        // Move forward
        if (isInsideBoard(r - 1, c) && board[(r - 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r - 1) * 8 + c;

            if (info.legalityRespected(square, newSq)) {
                if (r == 1) {
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::WHITE_QUEEN));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::WHITE_ROOK));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::WHITE_BISHOP));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::WHITE_KNIGHT));
                } else {
                    moves.push_back(Move::quiet(square, newSq, piece));
                }
            }

            // Double move from starting position
            newSq = (r - 2) * 8 + c;
            if (info.legalityRespected(square, newSq)) {
                if (r == 6 && board[(r - 2) * 8 + c] == Piece::EMPTY) {
                    moves.push_back(
                        Move::doublePush(square, (r - 2) * 8 + c, piece));
                }
            }
        }

        // Capture diagonally
        if (isInsideBoard(r - 1, c - 1) &&
            getPieceColor(board[(r - 1) * 8 + (c - 1)]) == Color::BLACK) {
            int newSq = (r - 1) * 8 + (c - 1);

            if (info.legalityRespected(square, newSq)) {
                if (r == 1) {
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_QUEEN,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(
                        square, newSq, piece, Piece::WHITE_ROOK, board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_BISHOP,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_KNIGHT,
                                                           board[newSq]));
                } else {
                    moves.push_back(
                        Move::capture(square, newSq, piece, board[newSq]));
                }
            }
        }
        if (isInsideBoard(r - 1, c + 1) &&
            getPieceColor(board[(r - 1) * 8 + (c + 1)]) == Color::BLACK) {
            int newSq = (r - 1) * 8 + (c + 1);

            if (info.legalityRespected(square, newSq)) {
                if (r == 1) {
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_QUEEN,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(
                        square, newSq, piece, Piece::WHITE_ROOK, board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_BISHOP,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::WHITE_KNIGHT,
                                                           board[newSq]));
                } else {
                    moves.push_back(
                        Move::capture(square, newSq, piece, board[newSq]));
                }
            }
        }
    } else {
        // Move forward
        if (isInsideBoard(r + 1, c) && board[(r + 1) * 8 + c] == Piece::EMPTY) {
            int newSq = (r + 1) * 8 + c;

            if (info.legalityRespected(square, newSq)) {
                if (r == 6) {
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::BLACK_QUEEN));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::BLACK_ROOK));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::BLACK_BISHOP));
                    moves.push_back(Move::quietPromotion(square, newSq, piece,
                                                         Piece::BLACK_KNIGHT));
                } else {
                    moves.push_back(Move::quiet(square, newSq, piece));
                }
            }
            // Double move from starting position
            newSq = (r + 2) * 8 + c;
            if (info.legalityRespected(square, newSq)) {
                if (r == 1 && board[(r + 2) * 8 + c] == Piece::EMPTY) {
                    moves.push_back(
                        Move::doublePush(square, (r + 2) * 8 + c, piece));
                }
            }
        }

        // Capture diagonally
        if (isInsideBoard(r + 1, c - 1) &&
            getPieceColor(board[(r + 1) * 8 + (c - 1)]) == Color::WHITE) {
            int newSq = (r + 1) * 8 + (c - 1);

            if (info.legalityRespected(square, newSq)) {
                if (r == 6) {
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_QUEEN,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(
                        square, newSq, piece, Piece::BLACK_ROOK, board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_BISHOP,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_KNIGHT,
                                                           board[newSq]));
                } else {
                    moves.push_back(
                        Move::capture(square, newSq, piece, board[newSq]));
                }
            }
        }
        if (isInsideBoard(r + 1, c + 1) &&
            getPieceColor(board[(r + 1) * 8 + (c + 1)]) == Color::WHITE) {
            int newSq = (r + 1) * 8 + (c + 1);

            if (info.legalityRespected(square, newSq)) {
                if (r == 6) {
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_QUEEN,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(
                        square, newSq, piece, Piece::BLACK_ROOK, board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_BISHOP,
                                                           board[newSq]));
                    moves.push_back(Move::capturePromotion(square, newSq, piece,
                                                           Piece::BLACK_KNIGHT,
                                                           board[newSq]));
                } else {
                    moves.push_back(
                        Move::capture(square, newSq, piece, board[newSq]));
                }
            }
        }
    }
}

void Position::generateEnPassantMoves(vector<Move>& moves,
                                      const LegalityInfo& info) {
    if (enPassantSquare == -1) return;

    auto addIfLegal = [&](const Move& move) {
        StateInfo savedState;
        makeMove(move, savedState);
        bool legal = !inCheck(-colorToMove);
        undoMove(move, savedState);

        if (legal) moves.push_back(move);
    };

    int r = enPassantSquare / 8, c = enPassantSquare % 8;
    if (colorToMove == Color::WHITE) {
        if (isInsideBoard(r + 1, c - 1) &&
            board[(r + 1) * 8 + (c - 1)] == Piece::WHITE_PAWN) {
            addIfLegal(Move::enPassant((r + 1) * 8 + (c - 1), enPassantSquare,
                                       Piece::WHITE_PAWN, Piece::BLACK_PAWN));
        }
        if (isInsideBoard(r + 1, c + 1) &&
            board[(r + 1) * 8 + (c + 1)] == Piece::WHITE_PAWN) {
            addIfLegal(Move::enPassant((r + 1) * 8 + (c + 1), enPassantSquare,
                                       Piece::WHITE_PAWN, Piece::BLACK_PAWN));
        }
    } else {
        if (isInsideBoard(r - 1, c - 1) &&
            board[(r - 1) * 8 + (c - 1)] == Piece::BLACK_PAWN) {
            addIfLegal(Move::enPassant((r - 1) * 8 + (c - 1), enPassantSquare,
                                       Piece::BLACK_PAWN, Piece::WHITE_PAWN));
        }
        if (isInsideBoard(r - 1, c + 1) &&
            board[(r - 1) * 8 + (c + 1)] == Piece::BLACK_PAWN) {
            addIfLegal(Move::enPassant((r - 1) * 8 + (c + 1), enPassantSquare,
                                       Piece::BLACK_PAWN, Piece::WHITE_PAWN));
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

vector<Move> Position::generateLegalMoves() {
    int kingSquare = -1;
    for (int square = 0; square < 64; square++) {
        Piece piece = board[square];
        if (getPieceColor(piece) != colorToMove) continue;

        if (piece == Piece::WHITE_KING || piece == Piece::BLACK_KING) {
            kingSquare = square;
            break;
        }
    }
    const int r = kingSquare / 8, c = kingSquare % 8;

    // identify checkers and pinned pieces along rays
    int numCheckers = 0;
    Pin pinnedSquare[64]{};
    bool evasionSquares[64] = {false};

    for (auto& dir : kingDir) {
        int diagonalDir = (dir[0] != 0 && dir[1] != 0);

        bool foundFriendly = false;
        int friendlySquare = -1;

        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            Piece piece = board[newSquare];

            if (piece == Piece::EMPTY) {
            } else if (getPieceColor(piece) == colorToMove) {
                if (foundFriendly) break;
                foundFriendly = true;
                friendlySquare = newSquare;
            } else {
                bool validPiece = diagonalDir ? (piece == Piece::WHITE_BISHOP ||
                                                 piece == Piece::BLACK_BISHOP ||
                                                 piece == Piece::WHITE_QUEEN ||
                                                 piece == Piece::BLACK_QUEEN)
                                              : (piece == Piece::WHITE_ROOK ||
                                                 piece == Piece::BLACK_ROOK ||
                                                 piece == Piece::WHITE_QUEEN ||
                                                 piece == Piece::BLACK_QUEEN);

                if (validPiece) {
                    if (foundFriendly) {
                        // Pinned piece found
                        pinnedSquare[friendlySquare] = {true, dir[0], dir[1]};
                    } else {
                        // Checker found
                        numCheckers++;
                        while (newR != r || newC != c) {
                            evasionSquares[newR * 8 + newC] = true;
                            newR -= dir[0];
                            newC -= dir[1];
                        }
                    }
                }
                break;
            }

            newR += dir[0];
            newC += dir[1];
        }
    }

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;
            Piece piece = board[newSquare];
            bool isKnight =
                (piece == Piece::WHITE_KNIGHT || piece == Piece::BLACK_KNIGHT);

            if (isKnight && getPieceColor(piece) != colorToMove) {
                numCheckers++;
                evasionSquares[newSquare] = true;
            }
        }
    }

    if (colorToMove == Color::BLACK) {
        if (isInsideBoard(r + 1, c - 1) &&
            board[(r + 1) * 8 + (c - 1)] == Piece::WHITE_PAWN) {
            numCheckers++;
            evasionSquares[(r + 1) * 8 + (c - 1)] = true;
        }
        if (isInsideBoard(r + 1, c + 1) &&
            board[(r + 1) * 8 + (c + 1)] == Piece::WHITE_PAWN) {
            numCheckers++;
            evasionSquares[(r + 1) * 8 + (c + 1)] = true;
        }
    } else {
        if (isInsideBoard(r - 1, c - 1) &&
            board[(r - 1) * 8 + (c - 1)] == Piece::BLACK_PAWN) {
            numCheckers++;
            evasionSquares[(r - 1) * 8 + (c - 1)] = true;
        }
        if (isInsideBoard(r - 1, c + 1) &&
            board[(r - 1) * 8 + (c + 1)] == Piece::BLACK_PAWN) {
            numCheckers++;
            evasionSquares[(r - 1) * 8 + (c + 1)] = true;
        }
    }

    LegalityInfo info(kingSquare, numCheckers > 0, pinnedSquare,
                      evasionSquares);
    vector<Move> moves;
    generateCastlingMoves(moves);
    generateKingMoves(kingSquare, moves);

    if (numCheckers >= 2) return moves;

    generateEnPassantMoves(moves, info);

    for (int square = 0; square < 64; square++) {
        Piece piece = board[square];
        if (piece == Piece::EMPTY) continue;

        if (getPieceColor(piece) != colorToMove) continue;

        switch (piece) {
            case Piece::WHITE_PAWN:
            case Piece::BLACK_PAWN:
                generatePawnMoves(square, moves, info);
                break;
            case Piece::WHITE_KNIGHT:
            case Piece::BLACK_KNIGHT:
                generateKnightMoves(square, moves, info);
                break;
            case Piece::WHITE_BISHOP:
            case Piece::BLACK_BISHOP:
                generateBishopMoves(square, moves, info);
                break;
            case Piece::WHITE_ROOK:
            case Piece::BLACK_ROOK:
                generateRookMoves(square, moves, info);
                break;
            case Piece::WHITE_QUEEN:
            case Piece::BLACK_QUEEN:
                generateQueenMoves(square, moves, info);
                break;
            default:
                break;
        }
    }

    return moves;
}