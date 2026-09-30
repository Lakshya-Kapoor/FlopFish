#include <vector>

#include "game_state.hpp"

using namespace std;

LegalMoveCollector::LegalMoveCollector(GameState& gameState,
                                       const LegalityInfo& info,
                                       vector<Move>& moves)
    : gameState(gameState), info(info), moves(moves) {}

bool LegalMoveCollector::requiresBoardValidation(const Move& move) const {
    return move.type == MoveType::EN_PASSANT_CAPTURE ||
           move.movedPiece == Piece::WHITE_KING ||
           move.movedPiece == Piece::BLACK_KING;
}

void LegalMoveCollector::add(const Move& move) {
    if (requiresBoardValidation(move)) {
        StateInfo savedState;
        gameState.makeMove(move, savedState);
        bool legal = !gameState.inCheck(-gameState.getColorToMove());
        gameState.undoMove(move, savedState);

        if (legal) moves.push_back(move);
    } else {
        if (info.legalityRespected(move.fromSquare, move.toSquare))
            moves.push_back(move);
    }
}

void GameState::generateKnightMoves(int square, LegalMoveCollector& col) {
    int r = square / 8, c = square % 8;

    for (auto& dir : knightDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                col.add(Move::quiet(square, newSquare, board[square]));
            } else if (getPieceColor(board[newSquare]) !=
                       getPieceColor(board[square])) {
                col.add(Move::capture(square, newSquare, board[square],
                                      board[newSquare]));
            }
        }
    }
}

void GameState::generateBishopMoves(int square, LegalMoveCollector& col) {
    int r = square / 8, c = square % 8;

    for (auto& dir : bishopDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                col.add(Move::quiet(square, newSquare, board[square]));
            } else {
                if (getPieceColor(board[newSquare]) !=
                    getPieceColor(board[square])) {
                    col.add(Move::capture(square, newSquare, board[square],
                                          board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }

            newR += dir[0];
            newC += dir[1];
        }
    }
}

void GameState::generateRookMoves(int square, LegalMoveCollector& col) {
    int r = square / 8, c = square % 8;

    for (auto& dir : rookDir) {
        int newR = r + dir[0], newC = c + dir[1];
        while (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                col.add(Move::quiet(square, newSquare, board[square]));
            } else {
                if (getPieceColor(board[newSquare]) !=
                    getPieceColor(board[square])) {
                    col.add(Move::capture(square, newSquare, board[square],
                                          board[newSquare]));
                }
                break;  // Stop if we hit a piece
            }
            newR += dir[0];
            newC += dir[1];
        }
    }
}

void GameState::generateQueenMoves(int square, LegalMoveCollector& col) {
    generateBishopMoves(square, col);
    generateRookMoves(square, col);
}

void GameState::generateKingMoves(int square, LegalMoveCollector& col) {
    int r = square / 8, c = square % 8;

    for (auto& dir : kingDir) {
        int newR = r + dir[0], newC = c + dir[1];
        if (isInsideBoard(newR, newC)) {
            int newSquare = newR * 8 + newC;

            if (board[newSquare] == Piece::EMPTY) {
                col.add(Move::quiet(square, newSquare, board[square]));
            } else if (getPieceColor(board[newSquare]) !=
                       getPieceColor(board[square])) {
                col.add(Move::capture(square, newSquare, board[square],
                                      board[newSquare]));
            } else {
                continue;
            }
        }
    }
}

void GameState::generatePawnMoves(int square, LegalMoveCollector& col) {
    int r = square / 8, c = square % 8;
    Piece piece = board[square];

    bool white = piece == Piece::WHITE_PAWN;
    int forward = white ? -1 : 1;
    int startRank = white ? 6 : 1;
    int promotionRank = white ? 1 : 6;
    Piece queen = white ? Piece::WHITE_QUEEN : Piece::BLACK_QUEEN;
    Piece rook = white ? Piece::WHITE_ROOK : Piece::BLACK_ROOK;
    Piece bishop = white ? Piece::WHITE_BISHOP : Piece::BLACK_BISHOP;
    Piece knight = white ? Piece::WHITE_KNIGHT : Piece::BLACK_KNIGHT;

    auto addPawnMove = [&](int target, Piece captured) {
        if (r == promotionRank) {
            if (captured == Piece::EMPTY) {
                col.add(Move::quietPromotion(square, target, piece, queen));
                col.add(Move::quietPromotion(square, target, piece, rook));
                col.add(Move::quietPromotion(square, target, piece, bishop));
                col.add(Move::quietPromotion(square, target, piece, knight));
            } else {
                col.add(Move::capturePromotion(square, target, piece, queen,
                                               captured));
                col.add(Move::capturePromotion(square, target, piece, rook,
                                               captured));
                col.add(Move::capturePromotion(square, target, piece, bishop,
                                               captured));
                col.add(Move::capturePromotion(square, target, piece, knight,
                                               captured));
            }
        } else if (captured == Piece::EMPTY) {
            col.add(Move::quiet(square, target, piece));
        } else {
            col.add(Move::capture(square, target, piece, captured));
        }
    };

    int forwardRow = r + forward;
    if (isInsideBoard(forwardRow, c) &&
        board[forwardRow * 8 + c] == Piece::EMPTY) {
        addPawnMove(forwardRow * 8 + c, Piece::EMPTY);

        if (r == startRank) {
            int doubleRow = r + 2 * forward;
            if (board[doubleRow * 8 + c] == Piece::EMPTY) {
                col.add(Move::doublePush(square, doubleRow * 8 + c, piece));
            }
        }
    }

    for (int fileOffset : {-1, 1}) {
        int targetFile = c + fileOffset;
        if (!isInsideBoard(forwardRow, targetFile)) continue;

        int target = forwardRow * 8 + targetFile;
        if (board[target] != Piece::EMPTY &&
            getPieceColor(board[target]) != getPieceColor(piece)) {
            addPawnMove(target, board[target]);
        }
    }
}

void GameState::generateEnPassantMoves(LegalMoveCollector& col) {
    if (enPassantSquare == -1) return;

    int r = enPassantSquare / 8, c = enPassantSquare % 8;
    if (colorToMove == Color::WHITE) {
        if (isInsideBoard(r + 1, c - 1) &&
            board[(r + 1) * 8 + (c - 1)] == Piece::WHITE_PAWN) {
            col.add(Move::enPassant((r + 1) * 8 + (c - 1), enPassantSquare,
                                    Piece::WHITE_PAWN, Piece::BLACK_PAWN));
        }
        if (isInsideBoard(r + 1, c + 1) &&
            board[(r + 1) * 8 + (c + 1)] == Piece::WHITE_PAWN) {
            col.add(Move::enPassant((r + 1) * 8 + (c + 1), enPassantSquare,
                                    Piece::WHITE_PAWN, Piece::BLACK_PAWN));
        }
    } else {
        if (isInsideBoard(r - 1, c - 1) &&
            board[(r - 1) * 8 + (c - 1)] == Piece::BLACK_PAWN) {
            col.add(Move::enPassant((r - 1) * 8 + (c - 1), enPassantSquare,
                                    Piece::BLACK_PAWN, Piece::WHITE_PAWN));
        }
        if (isInsideBoard(r - 1, c + 1) &&
            board[(r - 1) * 8 + (c + 1)] == Piece::BLACK_PAWN) {
            col.add(Move::enPassant((r - 1) * 8 + (c + 1), enPassantSquare,
                                    Piece::BLACK_PAWN, Piece::WHITE_PAWN));
        }
    }
}

void GameState::generateCastlingMoves(LegalMoveCollector& col) {
    if (colorToMove == Color::WHITE) {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_KINGSIDE)) {
            if (board[61] == Piece::EMPTY && board[62] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::BLACK) &&
                !isSquareAttacked(61, Color::BLACK) &&
                !isSquareAttacked(62, Color::BLACK)) {
                col.add(Move::castling(CastlingRights::WHITE_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::WHITE_QUEENSIDE)) {
            if (board[59] == Piece::EMPTY && board[58] == Piece::EMPTY &&
                board[57] == Piece::EMPTY &&
                !isSquareAttacked(60, Color::BLACK) &&
                !isSquareAttacked(59, Color::BLACK) &&
                !isSquareAttacked(58, Color::BLACK)) {
                col.add(Move::castling(CastlingRights::WHITE_QUEENSIDE));
            }
        }
    } else {
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_KINGSIDE)) {
            if (board[5] == Piece::EMPTY && board[6] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::WHITE) &&
                !isSquareAttacked(5, Color::WHITE) &&
                !isSquareAttacked(6, Color::WHITE)) {
                col.add(Move::castling(CastlingRights::BLACK_KINGSIDE));
            }
        }
        if (castlingRightsContains(castlingRights,
                                   CastlingRights::BLACK_QUEENSIDE)) {
            if (board[3] == Piece::EMPTY && board[2] == Piece::EMPTY &&
                board[1] == Piece::EMPTY &&
                !isSquareAttacked(4, Color::WHITE) &&
                !isSquareAttacked(3, Color::WHITE) &&
                !isSquareAttacked(2, Color::WHITE)) {
                col.add(Move::castling(CastlingRights::BLACK_QUEENSIDE));
            }
        }
    }
}

vector<Move> GameState::generateLegalMoves() {
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
    LegalMoveCollector col(*this, info, moves);
    generateCastlingMoves(col);
    generateKingMoves(kingSquare, col);

    if (numCheckers >= 2) return moves;

    generateEnPassantMoves(col);

    for (int square = 0; square < 64; square++) {
        Piece piece = board[square];
        if (piece == Piece::EMPTY) continue;

        if (getPieceColor(piece) != colorToMove) continue;

        switch (piece) {
            case Piece::WHITE_PAWN:
            case Piece::BLACK_PAWN:
                generatePawnMoves(square, col);
                break;
            case Piece::WHITE_KNIGHT:
            case Piece::BLACK_KNIGHT:
                generateKnightMoves(square, col);
                break;
            case Piece::WHITE_BISHOP:
            case Piece::BLACK_BISHOP:
                generateBishopMoves(square, col);
                break;
            case Piece::WHITE_ROOK:
            case Piece::BLACK_ROOK:
                generateRookMoves(square, col);
                break;
            case Piece::WHITE_QUEEN:
            case Piece::BLACK_QUEEN:
                generateQueenMoves(square, col);
                break;
            default:
                break;
        }
    }

    return moves;
}