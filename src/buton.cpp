//
// Created by user on 24.05.2026.
//

#include "buton.hpp"

Buton::Buton(raylib::Rectangle rect, std::string&& text) :
    rect_buton(rect), text(text) {
    locale::butoane.push_back(this);
}

Buton::~Buton() {
    std::erase(locale::butoane, this);
}

bool Buton::Selectat() const {
    // Un buton este considerat "activ/selectat" dacă indexul curent din tastatură îi aparține
    // SAU dacă mouse-ul se află direct peste el.
    int index_curent = -1;
    for (size_t i = 0; i < locale::butoane.size(); ++i) {
        if (locale::butoane[i] == this) {
            index_curent = static_cast<int>(i);
            break;
        }
    }
    return index_curent == locale::buton_actual || rect_buton.CheckCollision(::GetMousePosition());
}

int Buton::buton_selectat() {
    for (size_t i = 0; i < locale::butoane.size(); i++) {
        // Verificăm strict coliziunea cu mouse-ul pentru prioritate la click
        if (locale::butoane[i]->rect_buton.CheckCollision(::GetMousePosition())) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Buton::UpdateAll() {
    if (locale::butoane.empty()) return;

    // 1. Gestionare Navigare din Tastatură (W / S / Up / Down)
    if (raylib::Keyboard::IsKeyPressed(KEY_W) || raylib::Keyboard::IsKeyPressed(KEY_UP)) {
        if (locale::buton_actual < 0) locale::buton_actual = 0;
        locale::buton_actual = (locale::buton_actual - 1 + locale::contor_local) % locale::contor_local;
    }
    else if (raylib::Keyboard::IsKeyPressed(KEY_S) || raylib::Keyboard::IsKeyPressed(KEY_DOWN)) {
        if (locale::buton_actual < 0) locale::buton_actual = -1;
        locale::buton_actual = (locale::buton_actual + 1) % locale::contor_local;
    }

    int buton_sub_mouse = buton_selectat();
    if (buton_sub_mouse != -1) {
        locale::buton_actual = buton_sub_mouse;
    }

    bool click_mouse = ::IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool apasat_enter = raylib::Keyboard::IsKeyPressed(KEY_ENTER) ||
                        raylib::Keyboard::IsKeyPressed(KEY_KP_ENTER) ||
                        raylib::Keyboard::IsKeyPressed(KEY_SPACE);

    if ((click_mouse || apasat_enter) && locale::buton_actual != -1) {
        Buton* buton_activ = locale::butoane[locale::buton_actual];
        if (apasat_enter || buton_activ->rect_buton.CheckCollision(::GetMousePosition())) {
            if (buton_activ->on_mouse_click) {
                buton_activ->on_mouse_click();
            }
        }
    }
}

void Buton::OnMouseClick(const std::function<void()>& f) {
    on_mouse_click = f;
}

void Buton::Draw() const {
    bool selectat = Selectat();
    Color culoareBaza = selectat ? SKYBLUE : LIGHTGRAY;
    Color culoareText = selectat ? BLUE : DARKGRAY;

    rect_buton.Draw(culoareBaza);

    int lungimeText = ::MeasureText(text.c_str(), 20);
    int textX = static_cast<int>(rect_buton.x + (rect_buton.width - lungimeText) / 2);
    int textY = static_cast<int>(rect_buton.y + (rect_buton.height - 20) / 2);

    ::DrawText(text.c_str(), textX, textY, 20, culoareText);
}

void Buton::DrawAll() {
    for (auto* b : locale::butoane) {b->Draw();}
}
