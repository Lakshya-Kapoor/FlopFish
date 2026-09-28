// Perft test runner for the modular chess engine.

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "position.hpp"
using namespace std;

struct PerftCase {
    string fen;
    vector<pair<int, U64>> expectedNodes;
};

U64 perft(Position& pos, int depth) {
    if (depth == 0) return 1;

    U64 nodes = 0;
    vector<Move> moves = pos.generateLegalMoves();

    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);
        nodes += perft(pos, depth - 1);
        pos.undoMove(move, savedState);
    }

    return nodes;
}

U64 perftDivide(Position& pos, int depth) {
    if (depth == 0) return 1;

    U64 totalNodes = 0;

    vector<Move> moves = pos.generateLegalMoves();
    for (const Move& move : moves) {
        StateInfo savedState;
        pos.makeMove(move, savedState);

        U64 nodes = perft(pos, depth - 1);

        pos.undoMove(move, savedState);

        cout << move.toString() << ": " << nodes << "\n";
        totalNodes += nodes;
    }

    return totalNodes;
}

bool runPerftCase(PerftCase& testCase) {
    bool passed = true;

    for (const auto& [depth, expected] : testCase.expectedNodes) {
        Position pos(testCase.fen);
        U64 actual = perft(pos, depth);

        if (actual != expected) {
            cerr << "FAIL depth " << depth << ": expected " << expected
                 << ", got " << actual << "\n";
            passed = false;
        } else {
            cout << "PASS depth " << depth << ": " << actual << "\n";
        }
    }

    return passed;
}

int main() {
    vector<PerftCase> testCases = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
         {{1, 20},
          {2, 400},
          {3, 8902},
          {4, 197281},
          {5, 4865609},
          {6, 119060324}}},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
         {{1, 14},
          {2, 191},
          {3, 2812},
          {4, 43238},
          {5, 674624},
          {6, 11030083},
          {7, 178633661}}},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
         {{1, 6},
          {2, 264},
          {3, 9467},
          {4, 422333},
          {5, 15833292},
          {6, 706045033}}},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
         {{1, 44}, {2, 1486}, {3, 62379}, {4, 2103487}, {5, 89941194}}},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - "
         "0 10",
         {{1, 46}, {2, 2079}, {3, 89890}, {4, 3894594}, {5, 164075551}}},
    };

    bool allPassed = true;
    for (size_t index = 0; index < testCases.size(); index++) {
        cout << "Test position " << index + 1 << "\n";
        if (!runPerftCase(testCases[index])) allPassed = false;
    }

    // Position position(testCases[3].fen);
    // position.print();
    // cout << perftDivide(position, 1) << endl;

    return allPassed ? 0 : 1;
}
