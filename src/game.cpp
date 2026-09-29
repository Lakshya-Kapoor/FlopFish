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
    Position pos("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

    Config config;
    cout << "Enter search depth: ";
    cin >> config.depth;

    Player* p;
    int version;
    cout << "Enter FlopFish version (1 or 2 or 3 or 4): ";
    cin >> version;
    if (version == 1) {
        p = new FlopFishv1(config);
    } else if (version == 2) {
        p = new FlopFishv2(config);
    } else if (version == 3) {
        p = new FlopFishv3(config);
    } else if (version == 4) {
        p = new FlopFishv4(config);
    } else {
        cout << "Invalid version!" << endl;
        return 1;
    }

    Result res = p->getMove(pos);

    cout << "Best move: " << res.move.toString() << endl;
    cout << "Nodes visited: " << res.nodesVisited << endl;
    cout << "Time taken: " << res.timeTaken << " seconds" << endl;
    cout << "Nodes per second: " << res.nodesVisited / res.timeTaken << endl;
    // Config config;
    // config.depth = 7;
    // config.TTSize = 1 << 25;

    // Player* p1 = new FlopFishv4(config);

    // config.depth = 5;
    // Player* p2 = new FlopFishv3(config);

    // Game game(p1, p2);
    // game.play();
}