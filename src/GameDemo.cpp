#include "GameDemo.hpp"

std::vector<Bloc> GameDemo::ziduri;

GameDemo::GameDemo() : window(windowWidth, windowHeight, "Archer Duel", FLAG_WINDOW_RESIZABLE) {
    SetExitKey(KEY_NULL);
    window.SetMinSize(400, 300);
}

void GameDemo::ResetGame() {
    player = Caracter(arc, 0.1f, 0.0f, static_cast<float>(windowHeight) / 2.0f);
    inamic = Caracter(arc, 0.1f, static_cast<float>(windowWidth) - player.get_rect().width,
                      static_cast<float>(windowHeight) / 2.0f, "assets/textures/pacman_intors.png", 1.0f);
    stare = GameStates::StartMenu;
    stareUrm = GameStates::TuraPlayer;
    starePrev = GameStates::TuraPlayer;
    ziduri.clear();
}

void GameDemo::UpdateSageti(Caracter& p, Caracter& i, const std::vector<raylib::Rectangle>& rectangles, float dt, float max_height) {
    p.UpdateSagetiTrase(dt, rectangles, max_height);
    i.UpdateSagetiTrase(dt, rectangles, max_height);
}

void GameDemo::DeseneazaButon(raylib::Rectangle rect, const char* text, bool selectat) {
    Color culoareBaza = selectat ? SKYBLUE : LIGHTGRAY;
    Color culoareText = selectat ? BLUE : DARKGRAY;
    rect.Draw(culoareBaza);
    rect.DrawLines(selectat ? BLUE : GRAY, 3);
    int textWidth = MeasureText(text, 20);
    DrawText(text, static_cast<int>(rect.width / 2.0f) - textWidth / 2 + static_cast<int>(rect.x), static_cast<int>(rect.y) + 15, 20, culoareText);
}

void GameDemo::DeseneazaHUD() const {
    int fontSize = 20;
    int padding = 20;
    if (stare == TuraPlayer) {
        tipSageti tipP = player.TipUrmatoareaSageata();
        if (tipP != tipSageti::Invalid) {
            std::string textP = "Urmeaza: " + GetNumeTip(tipP);
            DrawText("TURA PLAYER 1", padding, padding, 15, DARKGRAY);
            DrawText(textP.c_str(), padding, padding + 20, fontSize, Sageata::get_color(tipP));
        }
    } else if (stare == TuraInamic) {
        tipSageti tipI = inamic.TipUrmatoareaSageata();
        if (tipI != tipSageti::Invalid) {
            std::string textI = "Urmeaza: " + GetNumeTip(tipI);
            int textWidth = MeasureText(textI.c_str(), fontSize);
            DrawText("TURA PLAYER 2", windowWidth - padding - MeasureText("TURA PLAYER 2", 15), padding, 15, DARKGRAY);
            DrawText(textI.c_str(), windowWidth - padding - textWidth, padding + 20, fontSize, Sageata::get_color(tipI));
        }
    }
}

void GameDemo::DeseneazaStartMenu() {
    constexpr int optiuni_start = 3;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) meniuSelectat = (meniuSelectat + 1) % optiuni_start;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) meniuSelectat = (meniuSelectat - 1 + optiuni_start) % optiuni_start;

    raylib::Rectangle btnStart(static_cast<float>(windowWidth) / 2.0f - 100.0f, 150.0f, 200.0f, 50.0f);
    raylib::Rectangle btnControale(static_cast<float>(windowWidth) / 2.0f - 100.0f, 230.0f, 200.0f, 50.0f);
    raylib::Rectangle btnExit(static_cast<float>(windowWidth) / 2.0f - 100.0f, 310.0f, 200.0f, 50.0f);

    if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (btnStart.CheckCollision(GetMousePosition()) || btnControale.CheckCollision(GetMousePosition()) || btnExit.CheckCollision(GetMousePosition())))) {
        if (meniuSelectat == 0) { stare = starePrev; joc_inceput = true; }
        else if (meniuSelectat == 1) stare = GameStates::Controale;
        else if (meniuSelectat == 2) close_window = true;
    }
    DeseneazaButon(btnStart, "START GAME", meniuSelectat == 0);
    DeseneazaButon(btnControale, "CONTROALE", meniuSelectat == 1);
    DeseneazaButon(btnExit, "EXIT", meniuSelectat == 2);
}

void GameDemo::DeseneazaPauseMenu() {
    constexpr int optiuni_pauza = 4;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) meniuSelectat = (meniuSelectat + 1) % optiuni_pauza;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) meniuSelectat = (meniuSelectat - 1 + optiuni_pauza) % optiuni_pauza;

    raylib::Rectangle btnResume(static_cast<float>(windowWidth) / 2.0f - 100.0f, 120.0f, 200.0f, 50.0f);
    raylib::Rectangle btnRestart(static_cast<float>(windowWidth) / 2.0f - 100.0f, 190.0f, 200.0f, 50.0f);
    raylib::Rectangle btnControale(static_cast<float>(windowWidth) / 2.0f - 100.0f, 260.0f, 200.0f, 50.0f);
    raylib::Rectangle btnExit(static_cast<float>(windowWidth) / 2.0f - 100.0f, 330.0f, 200.0f, 50.0f);

    if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && (btnResume.CheckCollision(GetMousePosition()) || btnRestart.CheckCollision(GetMousePosition()) || btnControale.CheckCollision(GetMousePosition()) || btnExit.CheckCollision(GetMousePosition())))) {
        if (meniuSelectat == 0) stare = starePrev;
        else if (meniuSelectat == 1) ResetGame();
        else if (meniuSelectat == 2) stare = GameStates::Controale;
        else if (meniuSelectat == 3) close_window = true;
    }
    DeseneazaButon(btnResume, "RESUME", meniuSelectat == 0);
    DeseneazaButon(btnRestart, "RESTART", meniuSelectat == 1);
    DeseneazaButon(btnControale, "CONTROALE", meniuSelectat == 2);
    DeseneazaButon(btnExit, "EXIT", meniuSelectat == 3);
}

void GameDemo::DeseneazaGameOver() {
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) meniuSelectat = (meniuSelectat + 1) % 2;
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;
    raylib::Rectangle btnPlayAgain(centerX - 100.0f, centerY, 200.0f, 50.0f);
    raylib::Rectangle btnExit(centerX - 100.0f, centerY + 70.0f, 200.0f, 50.0f);

    std::string titlu = "GAME OVER";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), 40) / 2, static_cast<int>(centerY) - 100, 40, RED);

    if (IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && (btnPlayAgain.CheckCollision(GetMousePosition()) || btnExit.CheckCollision(GetMousePosition())))) {
        if (meniuSelectat == 0) { stare = GameStates::StartMenu; ResetGame(); }
        else close_window = true;
    }
    DeseneazaButon(btnPlayAgain, "PLAY AGAIN", meniuSelectat == 0);
    DeseneazaButon(btnExit, "EXIT GAME", meniuSelectat == 1);
}

void GameDemo::DeseneazaControale() {
    int textX = windowWidth / 2 - 250;
    int textY = windowHeight / 2 - 120;
    DrawText("CONTROALE JOC", windowWidth / 2 - MeasureText("CONTROALE JOC", 30) / 2, textY - 60, 30, DARKGRAY);
    DrawText("ESCAPE: Deschide Main Menu.", textX, textY, 20, DARKGRAY);
    DrawText("LEFT CLICK: Pregateste tragerea cu arcul.", textX, textY + 55, 20, DARKGRAY);
    DrawText("C: Anuleaza actiunea de tragere.", textX, textY + 110, 20, DARKGRAY);
    DrawText("P: Spawneaza un perete defensiv.", textX, textY + 165, 20, DARKGRAY);
    raylib::Rectangle btnBack(static_cast<float>(windowWidth) / 2.0f - 100.0f, static_cast<float>(windowHeight) - 80.0f, 200.0f, 50.0f);
    DeseneazaButon(btnBack, "INAPOI", btnBack.CheckCollision(GetMousePosition()));
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && btnBack.CheckCollision(GetMousePosition())) stare = GameStates::StartMenu;
}

void GameDemo::Logica(Caracter& c1, Caracter& c2, float offset_zid, float dt, float max_height) {
    c1.DeseneazaCaracter(); c2.DeseneazaCaracter();
    std::vector<raylib::Rectangle> others;
    for (auto& b : ziduri) { others.emplace_back(b.get_rect()); b.Deseneaza(); }

    if (stare != GameStates::Intermediar) {
        c1.UpdateEfect();
        if (IsKeyPressed(KEY_P))
            ziduri.emplace_back(c1.get_rect().x + offset_zid, c1.get_rect().y - 10.0f, 15.0f, c1.get_rect().height);

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { trage_arc = true; forta_tragere = forta_de_baza; }
        if (trage_arc) {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                forta_tragere += viteza_trager * dt;
                forta_tragere = std::min(forta_tragere, max_forta_tragere);
            }
            if (IsKeyPressed(KEY_C)) { trage_arc = false; forta_tragere = 0.0f; }
            if (trage_arc && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
                c1.Trage(GetMousePosition(), forta_tragere, &c2);
                trage_arc = false; stare = GameStates::Intermediar;
            }
            // Traiectorie
            raylib::Vector2 pCenter = {c1.get_rect().x + c1.get_rect().width / 2.0f, c1.get_rect().y + c1.get_rect().height / 2.0f};
            raylib::Vector2 mousePos = GetMousePosition();
            float dx = mousePos.x - pCenter.x; float dy = mousePos.y - pCenter.y;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist > 0) {
                raylib::Vector2 simViteza = {(dx / dist) * forta_tragere, (dy / dist) * forta_tragere};
                raylib::Vector2 punctCurent = pCenter;
                for (int i = 0; i < 80; i++) {
                    simViteza.y += fizica::gravitate * 0.03f;
                    raylib::Vector2 punctUrmator = {punctCurent.x + simViteza.x * 0.03f, punctCurent.y + simViteza.y * 0.03f};
                    DrawLineEx(punctCurent, punctUrmator, 3.0f * (1.0f - static_cast<float>(i)/80.0f), ColorAlpha(RED, forta_tragere/max_forta_tragere + 0.2f));
                    punctCurent = punctUrmator;
                }
            }
        }
    } else { UpdateSageti(c1, c2, others, dt, max_height); }
}

bool GameDemo::FaraSageti(const Caracter& c1, const Caracter& c2) {
    return !(c1.AreSageti() || c2.AreSageti());
}

void GameDemo::run() {
    window.SetTargetFPS(60);
    while (!window.ShouldClose() && !close_window) {
        if (IsKeyPressed(KEY_ESCAPE) && stare != GameStates::StartMenu)
            stare = joc_inceput ? GameStates::PauseMenu : GameStates::StartMenu;

        if (window.GetWidth() != windowWidth || window.GetHeight() != windowHeight) {
            windowWidth = window.GetWidth(); windowHeight = window.GetHeight();
            player.set_pozitie(0.0f, static_cast<float>(windowHeight) / 2.0f);
            inamic.set_pozitie(static_cast<float>(windowWidth) - player.get_rect().width, static_cast<float>(windowHeight) / 2.0f);
        }
        if (!player.InViata() || !inamic.InViata()) stare = GameStates::GameOver;

        float dt = window.GetFrameTime();
        window.BeginDrawing(); window.ClearBackground(RAYWHITE);
        switch (stare) {
            case GameStates::GameOver: DeseneazaGameOver(); break;
            case GameStates::StartMenu: DeseneazaStartMenu(); break;
            case GameStates::TuraPlayer:
                stareUrm = GameStates::TuraInamic;
                if (FaraSageti(player, inamic)) { stare = GameStates::GameOver; break; }
                Logica(player, inamic, 50.0f + player.get_rect().width, dt, static_cast<float>(windowHeight));
                DeseneazaHUD(); break;
            case GameStates::Intermediar:
                Logica(player, inamic, 0, dt, static_cast<float>(windowHeight));
                if (TrecereTura) {
                    for (int i = static_cast<int>(ziduri.size()) - 1; i >= 0; --i) {
                        ziduri[static_cast<size_t>(i)].Update();
                        if (ziduri[static_cast<size_t>(i)].TrebuieSters()) ziduri.erase(ziduri.begin() + i);
                    }
                    stare = stareUrm; TrecereTura = false;
                }
                break;
            case GameStates::TuraInamic:
                stareUrm = GameStates::TuraPlayer;
                if (FaraSageti(player, inamic)) { stare = GameStates::GameOver; break; }
                Logica(inamic, player, -20.0f, dt, static_cast<float>(windowHeight));
                DeseneazaHUD(); break;
            case GameStates::Controale: DeseneazaControale(); break;
            case GameStates::PauseMenu: DeseneazaPauseMenu(); break;
        }
        window.EndDrawing();
    }
}

std::ostream& operator<<(std::ostream& os, const GameDemo& g) {
    os << "Marimea ferestrei jocului: " << g.windowWidth << "x" << g.windowHeight << std::endl;
    os << "Atributele caracterelor din joc:\nPlayer 1:\n" << g.player << "\nPlayer 2:\n" << g.inamic;
    return os;
}