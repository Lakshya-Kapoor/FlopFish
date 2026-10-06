#include "game.hpp"

#include <iostream>
#include <vector>

#include "move.hpp"

using namespace std;

Game::Game(Player* white, Player* black)
    : whitePlayer(white), blackPlayer(black) {}

Game::Game(Player* white, Player* black, const std::string& fen)
    : gameState(fen), whitePlayer(white), blackPlayer(black) {}

Player* Game::getCurrentPlayer() const {
    return gameState.getColorToMove() == Color::WHITE ? whitePlayer
                                                      : blackPlayer;
}

void Game::play() {
    int cnt = 1;
    while (true) {
        gameState.print();
        cout << cnt++ << endl;

        PositionState state = gameState.getPositionState();
        if (state == PositionState::CHECKMATE) {
            cout << (gameState.getColorToMove() == Color::WHITE ? "Black"
                                                                : "White")
                 << " wins by checkmate!" << endl;
            break;
        } else if (state == PositionState::STALEMATE) {
            cout << "Game ends in stalemate!" << endl;
            break;
        } else if (state == PositionState::DRAW_BY_HALFCLOCK) {
            cout << "Game ends in a draw by half clock !" << endl;
            break;
        } else if (state == PositionState::DRAW_BY_REPETITION) {
            cout << "Game ends in a draw by repetition !" << endl;
            break;
        }

        Player* currentPlayer = getCurrentPlayer();
        Move move = currentPlayer->getMove(gameState).move;
        gameState.makeMove(move);
    }
}

int main() {
    GameState gameState(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    Config config;
    cout << "Enter search depth: ";
    cin >> config.depth;
    config.TTSize = 1 << 20;

    Player* p;
    int version;
    cout << "Enter FlopFish version (1-5): ";
    cin >> version;
    if (version == 1) {
        p = new FlopFishv1(config);
    } else if (version == 2) {
        p = new FlopFishv2(config);
    } else if (version == 3) {
        p = new FlopFishv3(config);
    } else if (version == 4) {
        p = new FlopFishv4(config);
    } else if (version == 5) {
        p = new FlopFishv5(config);
    } else {
        cout << "Invalid version!" << endl;
        return 1;
    }

    Result res = p->getMove(gameState);

    cout << "Best move: " << res.move.toString() << endl;
    cout << "Nodes visited: " << res.nodesVisited << endl;
    cout << "Time taken: " << res.timeTaken << " seconds" << endl;
    cout << "Nodes per second: " << res.nodesVisited / res.timeTaken << endl;
    // Config config;
    // config.TTSize = 1 << 22;

    // config.depth = 7;
    // Player* p1 = new FlopFishv5(config);

    // config.depth = 7;
    // Player* p2 = new FlopFishv4(config);

    // Game game(p1, p2);
    // game.play();

    // delete p1;
    // delete p2;
}