#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <raylib-cpp.hpp>
namespace miscare {
    const char MoveKeys[5] = "WASD";
    raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}
class Caracter {
    raylib::Vector2 pos;
    const char* SpritePath;
public:
    Caracter() : pos(0, 0), SpritePath("textures/PrimavaraFrumoasaV2.png") {}
    Caracter(raylib::Vector2 pos) : pos(pos), SpritePath("textures/PrimavaraFrumoasaV2.png") {}
    Caracter(raylib::Vector2 pos, std::string filepath): pos(pos), SpritePath(filepath.c_str()) {}
    Caracter(raylib::Vector2 pos, const char* filepath): pos(pos), SpritePath(filepath) {}
    Caracter (const Caracter& other) {
        if (this!=&other) {
            pos = other.pos;
            SpritePath = other.SpritePath;
        }
    }
    Caracter& operator=(const Caracter& other) {
        if (this!=&other) {
            pos = other.pos;
            SpritePath = other.SpritePath;
        }
        return *this;
    }
    ~Caracter() = default;
    raylib::Vector2 get_pos() const {
        return pos;
    }
    raylib::Texture get_TextureAtPath() {return LoadTexture(SpritePath);}
    void Move() {
        for (int i = 0; i < 4; i++) {
            if (raylib::Keyboard::IsKeyDown(miscare::MoveKeys[i])) {
                pos.x+=miscare::ChangePos[i].x;
                pos.y+=miscare::ChangePos[i].y;
            }
        }
    }
};

int main() {
    raylib::Window window(800, 600, "test");
    Caracter p;
    raylib::Texture textura=p.get_TextureAtPath();
    window.SetTargetFPS(60);
    while (!window.ShouldClose()) {
        window.BeginDrawing();
        textura.Draw(p.get_pos());
        ClearBackground(RAYWHITE);
        window.EndDrawing();
        p.Move();
    }

    return 0;
}