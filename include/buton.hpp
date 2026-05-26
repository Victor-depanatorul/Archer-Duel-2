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
    std::function<void()> on_mouse_click;

    struct locale {
        static inline int contor_local = 0;
        static inline int buton_actual = 0;
        static inline std::vector<Buton*> butoane;
        locale() { contor_local++; }
        ~locale() { contor_local--; }
    } l;

    static void UpdateAll();
    [[nodiscard]] bool Selectat() const;
    static int buton_selectat();
    void Draw() const;
    static void DrawAll();

public:
    Buton() = delete;
    explicit Buton(raylib::Rectangle rect, std::string&& text);
    ~Buton();
    void OnMouseClick(const std::function<void()>& f);
    static void WorkInGame() {
        DrawAll();
        UpdateAll();
    }
};

#endif //OOP_BUTON_HPP