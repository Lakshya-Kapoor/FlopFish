#include <iostream>
#include <string>
#include <vector>

#include "game_state.hpp"

Move findMove(const std::vector<Move>& moves, const std::string& notation) {
    for (const Move& move : moves) {
        if (move.toString() == notation) return move;
    }

    std::cerr << "Move not found: " << notation << '\n';
    std::exit(1);
}

int main() {
    GameState gameState;
    const std::vector<std::string> sequence = {
        "g1f3", "g8f6", "f3g1", "f6g8", "g1f3", "g8f6", "f3g1", "f6g8",
    };

    for (const std::string& notation : sequence) {
        Move move = findMove(gameState.generateLegalMoves(), notation);
        StateInfo savedState;
        gameState.makeMove(move, savedState);
    }

    if (gameState.getPositionState() != PositionState::DRAW_BY_REPETITION) {
        std::cerr << "Threefold repetition was not detected\n";
        return 1;
    }

    GameState undoState;
    Move move = findMove(undoState.generateLegalMoves(), "g1f3");
    StateInfo savedState;
    U64 originalHash = undoState.getZobristHash();
    undoState.makeMove(move, savedState);
    undoState.undoMove(move, savedState);

    if (undoState.getZobristHash() != originalHash ||
        undoState.getPositionState() == PositionState::DRAW_BY_REPETITION) {
        std::cerr << "Repetition state was not restored after undo\n";
        return 1;
    }

    std::cout << "All repetition tests passed\n";
    return 0;
}