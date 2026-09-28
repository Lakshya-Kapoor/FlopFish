#include "zobrist_keys.hpp"

#include <random>
using namespace std;

ZobristKeys* ZobristKeys::getKeys() {
    if (_keys == nullptr) {
        _keys = new ZobristKeys();
    }
    return _keys;
}

ZobristKeys::ZobristKeys() {
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

U64 ZobristKeys::getPieceSquareKey(Piece piece, int square) {
    int pieceIndex = static_cast<int>(piece) - 1;
    return pieceSquareKeys[pieceIndex][square];
}

U64 ZobristKeys::getColorToMoveKey() { return colorToMoveKey; }

U64 ZobristKeys::getCastlingRightsKey(CastlingRights rights) {
    int rightsIndex = static_cast<int>(rights);
    return castlingRightsKeys[rightsIndex];
}

U64 ZobristKeys::getEnPassantFileKey(int enPassantSquare) {
    int file = enPassantSquare % 8;
    return enPassantFileKeys[file];
}