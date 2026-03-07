#include <iostream>
#include <array>
#include <chrono>
#include <thread>
#include <raylib-cpp.hpp>
#include <utility>
namespace miscare {
    const char MoveKeys[5] = "WASD";
    raylib::Vector2 ChangePos[4] = {{0, -10}, {-10, 0}, {0, 10}, {10, 0}};
}
enum tipSageti {
    Normala,
    Otravitoare,
    Aimbot,
    Healing,
    Giganta
};
class Entitate {
protected:
    raylib::Vector2 pos;
    raylib::Rectangle rect;
    raylib::Texture2D sprite;
public:
    Entitate() = delete;
    explicit Entitate(const raylib::Rectangle &rect) : rect(rect) {}
    Entitate(const raylib::Rectangle &rect,const raylib::Vector2 &pos) : pos(pos), rect(rect) {}
    Entitate(const raylib::Rectangle &rect, raylib::Texture2D &otherSprite)
        : rect(rect), sprite(std::move(otherSprite)) {}
    Entitate(const raylib::Vector2 &rect, const raylib::Vector2 pos, raylib::Texture2D &otherSprite) :
    rect(rect), pos(pos), sprite(std::move(otherSprite)) {}
    ~Entitate() = default;
    raylib::Vector2 get_pos() const {return pos;}
};
class Sageata : Entitate {
    tipSageti tip;
    double viteza;
};
class Arc {
    raylib::Vector2 pos;
    static const int CapacitateSageti=1000;
    std::array<Sageata, CapacitateSageti> capacitateSageti;
};
class Caracter  : Entitate {
    int hp=100;
public:
    raylib::Vector2 get_pos() const {
        return pos;
    }
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
    Entitate e(raylib::Rectangle(10, 10));
    std::cout<<e.get_pos().x << ' '  << e.get_pos().y << std::endl;
    // raylib::Texture textura=p.get_TextureAtPath();
    // window.SetTargetFPS(60);
    // while (!window.ShouldClose()) {
    //     window.BeginDrawing();
    //     textura.Draw(p.get_pos());
    //     ClearBackground(RAYWHITE);
    //     window.EndDrawing();
    //     p.Move();
    // }

    return 0;
}