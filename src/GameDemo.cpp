#include "GameDemo.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "bloc.hpp"
#include "buton.hpp"
#include "exceptii.hpp"

GameDemo::GameDemo() : p_up(0, 0, 0 ,0), window(windowWidth, windowHeight, "Archer Duel", FLAG_WINDOW_RESIZABLE) {
    SetExitKey(KEY_NULL);
    window.SetMinSize(400, 300);
    player1 = std::make_unique<Caracter>(Arc_factory::arc_default(), 0.1f, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
    player2 = std::make_unique<Caracter>(Arc_factory::arc_default(), 0.1f,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
    podea = player1->GetHitbox().y + player1->GetHitbox().height + 10;
    p_up = PowerUp(0.0f, static_cast<float>(windowWidth) - 10.0f,
                   0.0f, player1->GetHitbox().height - 100.0f);
    if (p_up.este_activ()) p_up.Consuma();
    p_up.TrySpawn();
    player_crt = player1.get();
}

GameDemo &GameDemo::get_GameDemo() {
    static GameDemo g;
    return g;
}

void GameDemo::ResetGame() {
    sageti_zbor.clear();

    game_modes_ = GameModes::Normal;

    stare = GameStates::StartMenu;
    stareUrm = GameStates::TuraPlayer;
    starePrev = GameStates::TuraPlayer;
}

void GameDemo::DeseneazaHUD() const {
    const auto* plr = dynamic_cast<Caracter*>(player_crt);
    const Caracter* other = player_crt == player1.get() ? player2.get() : player1.get();
    int fontSize = 20;
    int padding = 20;
    int offset_x_tura = 0, offset_x_hp[2] = {0}, offset_x_sageata = 0;
    std::string hpP ="HP:" + std::to_string(plr->get_hp());
    std::string hpI ="HP:" + std::to_string(other->get_hp());
    std::string textP = "Urmeaza: " + plr->TipUrmatoareaSageata();
    int textWidth = MeasureText(textP.c_str(), fontSize);

    if (player_crt == player1.get()) {
        offset_x_tura = padding;
        offset_x_hp[0] = padding;
        offset_x_hp[1] = windowWidth - padding - MeasureText(hpI.c_str(), fontSize);
        offset_x_sageata = padding;
        DrawText("TURA PLAYER 1", offset_x_tura, padding, fontSize - 5, DARKGRAY);
            DrawText(textP.c_str(), offset_x_sageata, padding + 20, fontSize, BLACK);
    }
    else {
        offset_x_tura = windowWidth - padding - MeasureText("TURA PLAYER 2", 15);
        offset_x_hp[1] = padding;
        offset_x_hp[0] = windowWidth - padding - MeasureText(hpI.c_str(), fontSize);
        offset_x_sageata = windowWidth - padding - textWidth;
        DrawText("TURA PLAYER 2", offset_x_tura, padding, 15, DARKGRAY);
            DrawText(textP.c_str(), offset_x_sageata, padding + 20, fontSize, BLACK);
    }
    DrawText(hpP.c_str(), offset_x_hp[0],
        static_cast<int>(plr->GetHitbox().y) - padding, fontSize, RED);
    DrawText(hpI.c_str(), offset_x_hp[1],
        static_cast<int>(other->GetHitbox().y) - padding, fontSize, RED);
}

void GameDemo::DeseneazaStartMenu() {
    Buton start({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        150.0f, 200.0f, 50.0f}, "START GAME");
    Buton controale({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        230.0f, 200.0f, 50.0f}, "CONTROALE");
    Buton exit({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        310.0f, 200.0f, 50.0f}, "EXIT GAME");
    start.OnMouseClick([&]() { stare = GameStates::MeniuGameModes;});
    controale.OnMouseClick([&](){stare = GameStates::Controale;});
    exit.OnMouseClick([&](){close_window = true;});
        Buton::WorkInGame();
}

void GameDemo::DeseneazaPauseMenu() {
    Buton resume({
        static_cast<float>(windowWidth) / 2.0f - 100.0f, 120.0f, 200.0f, 50.0f
    }, "RESUME");
    Buton restart({
        static_cast<float>(windowWidth) / 2.0f - 100.0f, 190.0f, 200.0f, 50.0f
    }, "RESTART");
    Buton controale({
    static_cast<float>(windowWidth) / 2.0f - 100.0f, 260.0f, 200.0f, 50.0f
    }, "CONTROALE");
    Buton exit({
        static_cast<float>(windowWidth) / 2.0f - 100.0f, 330.0f, 200.0f, 50.0f
    }, "EXIT GAME");

    resume.OnMouseClick([&](){stare = starePrev;});
    restart.OnMouseClick([this]() {this->ResetGame();});
    controale.OnMouseClick([&]() {stare = GameStates::Controale;});
    exit.OnMouseClick([&](){close_window = true;});

    Buton::WorkInGame();
}

GameDemo::Castigator GameDemo::determina_castigator() const {
    const bool p1_viu = player1->InViata();
    const bool p2_viu = player2->InViata();

    if (p1_viu && !p2_viu) return Castigator::Player1;
    if (p2_viu && !p1_viu) return Castigator::Player2;

    const bool p1_sageti = player1->AreSageti();
    const bool p2_sageti = player2->AreSageti();

    // Unul mai are sageti, celalalt nu -> castiga cel cu sageti
    if (p1_sageti && !p2_sageti) return Castigator::Player1;
    if (p2_sageti && !p1_sageti) return Castigator::Player2;

    if (player1->get_hp() > player2->get_hp()) return Castigator::Player1;
    if (player2->get_hp() > player1->get_hp()) return Castigator::Player2;
    return Castigator::Egalitate;
}

void GameDemo::DeseneazaGameOver() {
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;
    const std::string titlu = "GAME OVER";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), 40) / 2, static_cast<int>(centerY) - 100, 40, RED);

    std::string mesaj;
    switch (determina_castigator()) {
        case Castigator::Player1:   mesaj = "PLAYER 1 A CASTIGAT"; break;
        case Castigator::Player2:   mesaj = "PLAYER 2 A CASTIGAT"; break;
        case Castigator::Egalitate: mesaj = "EGALITATE"; break;
    }
    DrawText(mesaj.c_str(), static_cast<int>(centerX) - MeasureText(mesaj.c_str(), 30) / 2, static_cast<int>(centerY) - 50, 30, BLACK);

    Buton play_again({
        centerX - 100.0f, centerY, 200.0f, 50.0f
    }, "PLAY AGAIN");
    Buton exit({
        centerX - 100.0f, centerY + 70.0f, 200.0f, 50.0f
    }, "EXIT GAME");

    play_again.OnMouseClick([&](){stare = starePrev; ResetGame();});
    exit.OnMouseClick([&](){close_window = true;});

    Buton::WorkInGame();
}

void GameDemo::DeseneazaControale() {
    int textX = windowWidth / 2 - 250;
    int textY = windowHeight / 2 - 120;
    DrawText("CONTROALE JOC", windowWidth / 2 - MeasureText("CONTROALE JOC", 30) / 2, textY - 60, 30, DARKGRAY);
    DrawText("ESCAPE: Deschide Main Menu.", textX, textY, 20, DARKGRAY);
    DrawText("LEFT CLICK: Pregateste tragerea cu arcul.", textX, textY + 55, 20, DARKGRAY);
    DrawText("C: Anuleaza actiunea de tragere.", textX, textY + 110, 20, DARKGRAY);
    DrawText("P: Spawneaza un perete defensiv.", textX, textY + 165, 20, DARKGRAY);
    Buton back({
        static_cast<float>(windowWidth) / 2.0f - 100.0f, static_cast<float>(windowHeight) - 80.0f, 200.0f, 50.0f
    }, "BACK");
    back.OnMouseClick([&](){stare = !joc_inceput ? GameStates::StartMenu : GameStates::PauseMenu;});
    Buton::WorkInGame();
}

void GameDemo::DeseneazaGameMode() {
    Buton normal({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        150.0f, 200.0f, 50.0f}, "NORMAL");
    Buton randomized({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        230.0f, 200.0f, 50.0f}, "RANDOMIZED");
    Buton beserker({static_cast<float>(windowWidth) / 2.0f - 100.0f,
        310.0f, 200.0f, 50.0f}, "BESERKER");

    normal.OnMouseClick([&]() {
        game_modes_ = GameModes::Normal;
        player1 = std::make_unique<Caracter>(Arc_factory::arc_default(), 0.1f, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::arc_default(), 0.1f,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        player_crt = player1.get();
        stare = starePrev;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    randomized.OnMouseClick([&]() {
        game_modes_ = GameModes::Randomized;
        player1 = std::make_unique<Caracter>(Arc_factory::arc_random(), 0.1f, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::arc_random(), 0.1f,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        player_crt = player1.get();
        stare = starePrev;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    beserker.OnMouseClick([&]() {
        game_modes_ = GameModes::Beserker;
        player1 = std::make_unique<Caracter>(Arc_factory::beserker(), 0.1f, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::beserker(), 0.1f,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        p_up.SetProbability(1);
        player_crt = player1.get();
        stare = starePrev;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    Buton::WorkInGame();
}

void GameDemo::Logica(Caracter* c1, const Caracter* c2, float offset_zid, float dt) {
    auto& lista_entitati = Entitate::get_entitati();
    size_t nr_entitati = lista_entitati.size();

    for (auto* e : lista_entitati) e->Update(dt);

    for (auto& s : sageti_zbor) {
        s->update(dt, lista_entitati);
        s->Draw();
    }

    for (size_t i = 0; i < nr_entitati; ++i)
        for (size_t j = i + 1; j < nr_entitati; ++j)
            lista_entitati[i]->GetCollision(*lista_entitati[j]);

    if (std::erase_if(sageti_zbor, [](const auto& s){ return s->trebuie_stearsa(); }) > 0)
        TrecereTura = true;

    for (int i = static_cast<int>(lista_entitati.size()) - 1; i >= 0; --i) {
        const auto* b = dynamic_cast<Bloc*>(lista_entitati[i]);
        if (b != nullptr && b->TrebuieSters())
            delete lista_entitati[i];
    }

    if (stare != GameStates::Intermediar) {
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
            c1->IncearcaMiscare(raylib::Vector2{c1->GetHitbox().x + 100.0f, c1->GetHitbox().y});
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
            c1->IncearcaMiscare(raylib::Vector2{c1->GetHitbox().x - 100.0f, c1->GetHitbox().y});
        if (IsKeyPressed(KEY_P)) {
            float spawnX = c1->GetHitbox().x + offset_zid;
            float spawnY = c1->GetHitbox().y;
            new Bloc(spawnX, spawnY, latime_zid, c1->GetHitbox().height + 10.0f);
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!trage_arc) {
                trage_arc = true;
                forta_tragere = forta_de_baza;
            } else {
                auto s = c1->Trage(GetMousePosition(), forta_tragere, c2);
                if (s != nullptr)
                    sageti_zbor.push_back(std::move(s));
                trage_arc = false;
                stare = GameStates::Intermediar;
            }
        }

        if (trage_arc) {
            float miscareRotita = GetMouseWheelMove();
            if (miscareRotita != 0.0f) {
                forta_tragere += miscareRotita * 50.0f;
                if (forta_tragere > max_forta_tragere) forta_tragere = max_forta_tragere;
                if (forta_tragere < forta_de_baza) forta_tragere = forta_de_baza;
            }
            if (IsKeyPressed(KEY_C)) { trage_arc = false; forta_tragere = 0.0f; }
            raylib::Vector2 pCenter = {c1->GetHitbox().x + c1->GetHitbox().width / 2.0f,
                                       c1->GetHitbox().y + c1->GetHitbox().height / 2.0f};
            raylib::Vector2 mousePos = GetMousePosition();
            float dx = mousePos.x - pCenter.x; float dy = mousePos.y - pCenter.y;
            float dist = std::sqrt(dx*dx + dy*dy);
            if (dist > 0) {
                raylib::Vector2 simViteza = {(dx / dist) * forta_tragere, (dy / dist) * forta_tragere};
                raylib::Vector2 punctCurent = pCenter;
                for (int i = 0; i < 50; i++) {
                    simViteza.y += fizica::gravitate * 0.03f;
                    raylib::Vector2 punctUrmator = {punctCurent.x + simViteza.x * 0.03f,
                                                    punctCurent.y + simViteza.y * 0.03f};
                    DrawLineEx(punctCurent, punctUrmator, 3.0f * (1.0f - static_cast<float>(i)/80.0f),
                               ColorAlpha(RED, forta_tragere/max_forta_tragere + 0.2f));
                    punctCurent = punctUrmator;
                }
            }
        }
    }
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
            player1->SetPosition(0.0f, static_cast<float>(windowHeight) / 2.0f);
            player2->SetPosition(static_cast<float>(windowWidth) - player1->GetHitbox().width, static_cast<float>(windowHeight) / 2.0f);
        }
        if (!player1->InViata() || !player2->InViata()) stare = GameStates::GameOver;

        if (nr_ture >= 20) p_up.SetProbability(10);
        float dt = window.GetFrameTime();
        podea = player1->GetHitbox().y + player1->GetHitbox().height + 10;
        Caracter* alt_player = player_crt == player1.get() ? player2.get() : player1.get();
        window.BeginDrawing(); window.ClearBackground(RAYWHITE);
        if (stare == GameStates::TuraPlayer || stare == GameStates::Intermediar) {
            for (auto* e : Entitate::get_entitati()) e->Draw();
            DrawLine(0, static_cast<int>(podea), windowWidth, static_cast<int>(podea), BLACK);
        }
        switch (stare) {
            case GameStates::GameOver: DeseneazaGameOver(); break;
            case GameStates::StartMenu: DeseneazaStartMenu(); break;
            case GameStates::MeniuGameModes: DeseneazaGameMode(); break;
            case GameStates::TuraPlayer: {
                starePrev = GameStates::TuraPlayer;
                stareUrm = GameStates::TuraPlayer;
                if (FaraSageti(*player1, *player2)) { stare = GameStates::GameOver; break; }
                float offset_zid = player_crt == player1.get() ?
                distanta_zid + player_crt->GetHitbox().width :
                -(distanta_zid + latime_zid);
                Logica(player_crt, alt_player, offset_zid, dt);
                DeseneazaHUD(); break;
            }

            case GameStates::Intermediar:
                starePrev = GameStates::Intermediar;
                Logica(player_crt, alt_player, 0, dt);
                if (TrecereTura) {
                    player_crt = alt_player;
                    if (p_up.este_activ()) p_up.Consuma();
                    p_up.TrySpawn();
                    stare = stareUrm; TrecereTura = false;
                    nr_ture++;
                }
                break;
            case GameStates::Controale: DeseneazaControale(); break;
            case GameStates::PauseMenu: DeseneazaPauseMenu(); break;
            default: throw eroare_stare(static_cast<int>(stare));
        }
        window.EndDrawing();
    }
}

std::ostream& operator<<(std::ostream& os, const GameDemo& g) {
    os << "Marimea ferestrei jocului: " << g.windowWidth << "x" << g.windowHeight << std::endl;
    os << "Atributele caracterelor din joc:\nPlayer1 1:\n" << *g.player1 << "\nPlayer1 2:\n" << *g.player2;
    return os;
}