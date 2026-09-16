#include "game.hpp"

#include <iostream>
#include <vector>

#include "move.hpp"

using namespace std;

Game::Game(Player* white, Player* black)
    : whitePlayer(white), blackPlayer(black) {}

Game::Game(Player* white, Player* black, const std::string& fen)
    : pos(fen), whitePlayer(white), blackPlayer(black) {}

Player* Game::getCurrentPlayer() const {
    return pos.getColorToMove() == Color::WHITE ? whitePlayer : blackPlayer;
}

void Game::play() {
    int cnt = 1;
    while (true) {
        pos.print();
        cout << cnt++ << endl;

        PositionState state = pos.getPositionState();
        if (state == PositionState::CHECKMATE) {
            cout << (pos.getColorToMove() == Color::WHITE ? "Black" : "White")
                 << " wins by checkmate!" << endl;
            break;
        } else if (state == PositionState::STALEMATE) {
            cout << "Game ends in stalemate!" << endl;
            break;
        } else if (state == PositionState::DRAW) {
            cout << "Game ends in a draw!" << endl;
            break;
        }

        Player* currentPlayer = getCurrentPlayer();
        Move move = currentPlayer->getMove(pos).move;
        pos.makeMove(move);
    }
}

int main() {
    // Position pos(
    //     "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - -
    //     0 " "10");
    // Config config;
    // cout << "Enter search depth: ";
    // cin >> config.depth;

    // cout << "Reorder moves? (1 for yes, 0 for no): ";
    // cin >> config.reorderMoves;

    // if (config.reorderMoves) {
    //     cout << "Reorder captures? (1 for yes, 0 for no): ";
    //     cin >> config.reorderCaptures;
    // } else {
    //     config.reorderCaptures = false;
    // }

    // FlopFishv1 p1(config);
    // Result res = p1.getMove(pos);

    // cout << "Best move: " << res.move.toString() << endl;
    // cout << "Nodes visited: " << res.nodesVisited << endl;

    FlopFishv1 p1({depth : 3, reorderMoves : true, reorderCaptures : true});
    FlopFishv1 p2({depth : 3, reorderMoves : true, reorderCaptures : true});
    Game game(&p1, &p2);
    game.play();
}