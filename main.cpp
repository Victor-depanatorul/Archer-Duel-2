#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <raylib-cpp.hpp>


int main() {
    raylib::Window window(800, 600, "test");
    raylib::Texture textura("textures/PrimavaraFrumoasaV2.png");
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        ClearBackground(RAYWHITE);
        const int x = (window.GetWidth()>>1) - (textura.GetWidth()>>1);
        const int y = (window.GetHeight()>>1) - (textura.GetHeight()>>1);
        textura.Draw(x, y);
        window.EndDrawing();
    }
    return 0;
}