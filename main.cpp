#include "GameDemo.hpp"

int main() {
    auto& game = GameDemo::get_GameDemo();
    std::cout << game;
    game.run();
    return 0;
}