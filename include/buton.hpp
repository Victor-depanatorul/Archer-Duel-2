//
// Created by user on 24.05.2026.
//

#ifndef OOP_BUTON_HPP
#define OOP_BUTON_HPP
#include <functional>

#include "basic_includes.hpp"

class Buton {
    raylib::Rectangle rect_buton;
    std::string text;
    std::string descriere;
    std::function<void()> on_mouse_click;

        static inline int contor = 0;
        static inline int buton_actual = 0;
        static inline std::vector<Buton*> butoane;

    static void UpdateAll();
    [[nodiscard]] bool Selectat() const;
    static int buton_selectat();
    void Draw() const;
    static void DrawAll();

public:
    Buton() = delete;
    explicit Buton(raylib::Rectangle rect, std::string&& text, std::string descriere = "");
    ~Buton();
    void OnMouseClick(const std::function<void()>& f);
    static void WorkInGame() {
        DrawAll();
        UpdateAll();
    }
};

#endif //OOP_BUTON_HPP