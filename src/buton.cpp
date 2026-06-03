//
// Created by user on 24.05.2026.
//

#include "buton.hpp"

Buton::Buton(raylib::Rectangle rect, std::string&& text) :
    rect_buton(rect), text(text) {
    contor++;
    butoane.push_back(this);
}

Buton::~Buton() {
    std::erase(butoane, this);
}

bool Buton::Selectat() const {
    // Un buton este considerat "activ/selectat" dacă indexul curent din tastatură îi aparține
    // SAU dacă mouse-ul se află direct peste el.
    int index_curent = -1;
    for (size_t i = 0; i < butoane.size(); ++i) {
        if (butoane[i] == this) {
            index_curent = static_cast<int>(i);
            break;
        }
    }
    return index_curent == buton_actual || rect_buton.CheckCollision(::GetMousePosition());
}

int Buton::buton_selectat() {
    for (size_t i = 0; i < butoane.size(); i++) {
        // Verificăm strict coliziunea cu mouse-ul pentru prioritate la click
        if (butoane[i]->rect_buton.CheckCollision(::GetMousePosition())) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void Buton::UpdateAll() {
    if (butoane.empty()) return;

    // 1. Gestionare Navigare din Tastatură (W / S / Up / Down)
    if (raylib::Keyboard::IsKeyPressed(KEY_W) || raylib::Keyboard::IsKeyPressed(KEY_UP)) {
        if (buton_actual < 0) buton_actual = 0;
        buton_actual = (buton_actual - 1 + contor) % contor;
    }
    else if (raylib::Keyboard::IsKeyPressed(KEY_S) || raylib::Keyboard::IsKeyPressed(KEY_DOWN)) {
        if (buton_actual < 0) buton_actual = -1;
        buton_actual = (buton_actual + 1) % contor;
    }

    int buton_sub_mouse = buton_selectat();
    if (buton_sub_mouse != -1) {
        buton_actual = buton_sub_mouse;
    }

    bool click_mouse = ::IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool apasat_enter = raylib::Keyboard::IsKeyPressed(KEY_ENTER) ||
                        raylib::Keyboard::IsKeyPressed(KEY_KP_ENTER) ||
                        raylib::Keyboard::IsKeyPressed(KEY_SPACE);

    if ((click_mouse || apasat_enter) && buton_actual != -1) {
        Buton* buton_activ = butoane[buton_actual];
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

    // Fontul textului se scaleaza dupa inaltimea butonului (50 -> 20).
    int fontSize = std::max(1, static_cast<int>(rect_buton.height * 0.4f));
    int lungimeText = ::MeasureText(text.c_str(), fontSize);
    int textX = static_cast<int>(rect_buton.x + (rect_buton.width - static_cast<float>(lungimeText)) / 2);
    int textY = static_cast<int>(rect_buton.y + (rect_buton.height - static_cast<float>(fontSize)) / 2);

    ::DrawText(text.c_str(), textX, textY, fontSize, culoareText);
}

void Buton::DrawAll() {
    for (const auto* b : butoane) {b->Draw();}
}
