#include <iostream>
#include <string>
#include <vector>

#include "position.hpp"

using namespace std;

bool verifyPosition(const string& fen) {
    Position position(fen);
    U64 originalHash = position.getZobristHash();
    vector<Move> moves = position.generateLegalMoves();

    for (const Move& move : moves) {
        StateInfo savedState;
        position.makeMove(move, savedState);

        if (position.getZobristHash() != position.generateZobristHash()) {
            cerr << "Hash mismatch after move " << move.toString() << " in "
                 << fen << '\n';
            return false;
        }

        position.undoMove(move, savedState);

        if (position.getZobristHash() != originalHash ||
            position.getZobristHash() != position.generateZobristHash()) {
            cerr << "Hash mismatch after undoing move " << move.toString()
                 << " in " << fen << '\n';
            return false;
        }
    }

    return true;
}

int main() {
    const vector<string> testPositions = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1",
        "4k3/8/8/3p4/2B5/8/8/4K3 w - - 0 1",
        "4k3/P7/8/8/8/8/8/4K3 w - - 0 1",
        "4k3/8/8/3pP3/8/8/8/4K3 w - d6 0 1",
    };

    for (const string& fen : testPositions) {
        if (!verifyPosition(fen)) return 1;
    }

    cout << "All zobrist hash tests passed\n";
    return 0;
}