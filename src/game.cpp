#include "../include/game.hpp"

#include <iostream>
#include <vector>

#include "../include/move.hpp"

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
        Move move = currentPlayer->getMove(pos);
        pos.makeMove(move);
    }
}

int main() {
    Game game(new FlopFishv1(), new FlopFishv1());
    game.play();
}