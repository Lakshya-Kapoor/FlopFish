#include "zobrist.hpp"

#include <random>
using namespace std;

namespace {
struct ZobristInitializer {
    ZobristInitializer() { Zobrist::initZobristKeys(); }
};

const ZobristInitializer zobristInitializer;
}  // namespace

U64 Zobrist::pieceSquareKeys[12][64];
U64 Zobrist::colorToMoveKey;
U64 Zobrist::castlingRightsKeys[16];
U64 Zobrist::enPassantFileKeys[8];

void Zobrist::initZobristKeys() {
    mt19937_64 rng(42);

    for (int piece = 0; piece < 12; piece++) {
        for (int square = 0; square < 64; square++) {
            pieceSquareKeys[piece][square] = rng();
        }
    }

    colorToMoveKey = rng();

    for (int rights = 0; rights < 16; rights++) {
        castlingRightsKeys[rights] = rng();
    }

    for (int file = 0; file < 8; file++) {
        enPassantFileKeys[file] = rng();
    }
}

U64 Zobrist::getPieceSquareKey(Piece piece, int square) {
    int pieceIndex = static_cast<int>(piece) - 1;
    return pieceSquareKeys[pieceIndex][square];
}

U64 Zobrist::getColorToMoveKey() { return colorToMoveKey; }

U64 Zobrist::getCastlingRightsKey(CastlingRights rights) {
    int rightsIndex = static_cast<int>(rights);
    return castlingRightsKeys[rightsIndex];
}

U64 Zobrist::getEnPassantFileKey(int enPassantSquare) {
    int file = enPassantSquare % 8;
    return enPassantFileKeys[file];
}