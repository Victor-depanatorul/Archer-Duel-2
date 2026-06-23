#include "Game.hpp"
#include "exceptii.hpp"

int main() {
    try {
        auto& game = Game::get_GameInstance();
        std::cout << game;
        game.run();
    } catch (const eroare_joc& e) {
        std::cerr << "Eroare de joc: " << e.what() << '\n';
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Eroare neasteptata: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
