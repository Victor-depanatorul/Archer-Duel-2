#include "GameDemo.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "bloc.hpp"
#include "buton.hpp"
#include "exceptii.hpp"
#include <algorithm>

GameDemo::GameDemo() : p_up(0, 0, 0 ,0),
window(windowWidth, windowHeight, "Archer Duel", FLAG_WINDOW_RESIZABLE) {
    SetExitKey(KEY_NULL);
    window.SetMinSize(400, 300);
    player1 = std::make_unique<Caracter>(Arc_factory::arc_default(), BASE_CHR_SCALE, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
    player2 = std::make_unique<Caracter>(Arc_factory::arc_default(), BASE_CHR_SCALE,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
    podea = player1->GetHitbox().y + player1->GetHitbox().height + 10;
    p_up = PowerUp(50.0f, static_cast<float>(windowWidth) - 50.0f,
                   0.0f, player1->GetHitbox().height - 200.0f);
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
    TrecereTura = false;
    auto& lista = Entitate::get_entitati();
    for (int i = static_cast<int>(lista.size()) - 1; i >= 0; --i) {
        if (dynamic_cast<Bloc*>(lista[i]) != nullptr)
            delete lista[i];
    }

    game_modes_ = GameModes::Normal;

    stare = GameStates::StartMenu;
    stareUrm = GameStates::TuraPlayer;
    starePrev = GameStates::TuraPlayer;
}

void GameDemo::DeseneazaHUD() const {
    const auto* plr = player_crt;
    const Caracter* other = player_crt == player1.get() ? player2.get() : player1.get();
    int fontSize = 20;
    int fontPuncte = fontSize - 2;   // putin mai mare decat inainte (era fontSize - 4)
    int padding = 20;
    int offset_x_tura = 0, offset_x_hp[2] = {0}, offset_x_sageata = 0;

    std::string hpP ="HP:" + std::to_string(plr->get_hp());
    std::string hpI ="HP:" + std::to_string(other->get_hp());
    std::string textP = "Urmeaza: " + plr->TipUrmatoareaSageata();
    Color culoare = plr->CuloareUrmatoareaSageate();
    int textWidth = MeasureText(textP.c_str(), fontSize);

    if (player_crt == player1.get()) {
        offset_x_tura = padding;
        offset_x_hp[0] = padding;
        offset_x_hp[1] = windowWidth - padding - MeasureText(hpI.c_str(), fontSize);
        offset_x_sageata = padding;
        DrawText("TURA PLAYER 1", offset_x_tura, padding, fontSize - 5, DARKGRAY);
            DrawText(textP.c_str(), offset_x_sageata, padding + 20, fontSize, culoare);
    }
    else {
        offset_x_tura = windowWidth - padding - MeasureText("TURA PLAYER 2", 15);
        offset_x_hp[1] = padding;
        offset_x_hp[0] = windowWidth - padding - MeasureText(hpI.c_str(), fontSize);
        offset_x_sageata = windowWidth - padding - textWidth;
        DrawText("TURA PLAYER 2", offset_x_tura, padding, 15, DARKGRAY);
            DrawText(textP.c_str(), offset_x_sageata, padding + 20, fontSize, culoare);
    }

    // HP + puncte, ridicate mai sus deasupra fiecarui caracter.
    int hpY_plr   = static_cast<int>(plr->GetHitbox().y)   - padding - fontSize - 20;
    int hpY_other = static_cast<int>(other->GetHitbox().y) - padding - fontSize - 20;
    DrawText(hpP.c_str(), offset_x_hp[0], hpY_plr,   fontSize, RED);
    DrawText(hpI.c_str(), offset_x_hp[1], hpY_other, fontSize, RED);

    // Puncte (float * 100, convertit in int), afisate chiar sub HP.
    std::string puncteP = "Puncte:" + std::to_string(static_cast<int>(plr->get_puncte() * 100));
    std::string puncteI = "Puncte:" + std::to_string(static_cast<int>(other->get_puncte() * 100));
    DrawText(puncteP.c_str(), offset_x_hp[0], hpY_plr + fontSize, fontPuncte, DARKGRAY);
    DrawText(puncteI.c_str(), offset_x_hp[1], hpY_other + fontSize, fontPuncte, DARKGRAY);
}

void GameDemo::DeseneazaStartMenu() {
    float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                           static_cast<float>(windowHeight) / BASE_HEIGHT);
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;
    float btnW = 200.0f * scale, btnH = 50.0f * scale;
    int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "ARCHER DUEL";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 165.0f * scale), fontTitlu, DARKGRAY);

    Buton start({centerX - btnW / 2, centerY - 105.0f * scale, btnW, btnH}, "START GAME");
    Buton controale({centerX - btnW / 2, centerY - 25.0f * scale, btnW, btnH}, "CONTROALE");
    Buton exit({centerX - btnW / 2, centerY + 55.0f * scale, btnW, btnH}, "EXIT GAME");
    start.OnMouseClick([&]() { stare = GameStates::MeniuGameModes;});
    controale.OnMouseClick([&](){stare = GameStates::Controale;});
    exit.OnMouseClick([&](){close_window = true;});
    Buton::WorkInGame();
}

void GameDemo::DeseneazaPauseMenu() {
    float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                           static_cast<float>(windowHeight) / BASE_HEIGHT);
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;
    float btnW = 200.0f * scale, btnH = 50.0f * scale;
    int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "PAUZA";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 195.0f * scale), fontTitlu, DARKGRAY);

    Buton resume({centerX - btnW / 2, centerY - 130.0f * scale, btnW, btnH}, "RESUME");
    Buton restart({centerX - btnW / 2, centerY - 60.0f * scale, btnW, btnH}, "RESTART");
    Buton controale({centerX - btnW / 2, centerY + 10.0f * scale, btnW, btnH}, "CONTROALE");
    Buton exit({centerX - btnW / 2, centerY + 80.0f * scale, btnW, btnH}, "EXIT GAME");

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
    const float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                                 static_cast<float>(windowHeight) / BASE_HEIGHT);
    const float centerX = static_cast<float>(windowWidth) / 2.0f;
    const float centerY = static_cast<float>(windowHeight) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale;
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const int fontMesaj = std::max(1, static_cast<int>(30 * scale));
    const std::string titlu = "GAME OVER";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2, static_cast<int>(centerY - 100.0f * scale), fontTitlu, RED);

    std::string mesaj;
    switch (determina_castigator()) {
        case Castigator::Player1:   mesaj = "PLAYER 1 A CASTIGAT"; break;
        case Castigator::Player2:   mesaj = "PLAYER 2 A CASTIGAT"; break;
        case Castigator::Egalitate: mesaj = "EGALITATE"; break;
    }
    DrawText(mesaj.c_str(), static_cast<int>(centerX) - MeasureText(mesaj.c_str(), fontMesaj) / 2, static_cast<int>(centerY - 50.0f * scale), fontMesaj, BLACK);

    Buton play_again({
        centerX - btnW / 2, centerY, btnW, btnH
    }, "PLAY AGAIN");
    Buton stats({
    centerX - btnW / 2, centerY + 70.0f * scale, btnW, btnH
    }, "VREAU STATISTICI");
    Buton exit({
        centerX - btnW / 2, centerY + 140.0f * scale, btnW, btnH
    }, "EXIT GAME");

    play_again.OnMouseClick([&](){stare = starePrev; ResetGame();});
    exit.OnMouseClick([&](){close_window = true;});
    stats.OnMouseClick([&](){stare = GameStates::MeniuStatistici;});
    Buton::WorkInGame();
}

void GameDemo::DeseneazaStats() {
    // Putin mai mare decat scalarea de baza pentru text si butoane.
    const float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                                 static_cast<float>(windowHeight) / BASE_HEIGHT) * 1.3f;
    const int centerX = windowWidth / 2;
    const int centerY = windowHeight / 2;

    // Doua coloane simetrice fata de centrul ecranului.
    const int colW = static_cast<int>(200 * scale);
    const int colGap = static_cast<int>(60 * scale);
    const int statsY = centerY - static_cast<int>(140 * scale);
    const int leftX = centerX - colGap / 2 - colW;
    const int rightX = centerX + colGap / 2;

    player1->get_stats().Deseneaza(leftX, statsY, BLACK, "Player 1:", scale);
    player2->get_stats().Deseneaza(rightX, statsY, BLACK, "Player 2:", scale);

    // Butonul GO BACK centrat, sub blocul de statistici (6 randuri a cate 25*scale).
    const float btnW = 200.0f * scale, btnH = 50.0f * scale;
    const float btnX = static_cast<float>(centerX) - btnW / 2.0f;
    const float btnY = static_cast<float>(statsY) + 6 * 25 * scale + 30.0f * scale;
    Buton back({btnX, btnY, btnW, btnH}, "GO BACK");
    back.OnMouseClick([&](){stare = GameStates::GameOver;});
    Buton::WorkInGame();
}

void GameDemo::DeseneazaControale() {

    float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                           static_cast<float>(windowHeight) / BASE_HEIGHT);

    int fontTitlu = std::max(1, static_cast<int>(30 * scale));
    int fontNormal = std::max(1, static_cast<int>(20 * scale));
    int fontMic = std::max(1, static_cast<int>(16 * scale));

    int textX = windowWidth / 2 - static_cast<int>(250 * scale);
    int textY = windowHeight / 2 - static_cast<int>(220 * scale);

    int latimeTitlu = MeasureText("CONTROALE JOC", fontTitlu);
    DrawText("CONTROALE JOC", windowWidth / 2 - latimeTitlu / 2, textY - static_cast<int>(60 * scale), fontTitlu, DARKGRAY);

    DrawText("ESCAPE: Deschide Main Menu.", textX, textY, fontNormal, DARKGRAY);
    DrawText("LEFT CLICK: Pregateste tragerea cu arcul.", textX, textY + static_cast<int>(55 * scale), fontNormal, DARKGRAY);
    DrawText("Mouse Wheel: Modifica forta de tragere.", textX, textY + static_cast<int>(110 * scale), fontNormal, DARKGRAY);
    DrawText("In sus, o creste, in jos, o scade", textX + static_cast<int>(20 * scale), textY + static_cast<int>(135 * scale), fontMic, GRAY);
    DrawText("C: Anuleaza actiunea de tragere.", textX, textY + static_cast<int>(190 * scale), fontNormal, DARKGRAY);
    DrawText("P: Spawneaza un perete defensiv.", textX, textY + static_cast<int>(245 * scale), fontNormal, DARKGRAY);
    DrawText("Z: Arunca sageata curenta din arc", textX, textY + static_cast<int>(300 * scale), fontNormal, DARKGRAY);
    DrawText("X: Schimba sageata curenta intr-una normala. Ai voie odata per tura", textX, textY + static_cast<int>(355 * scale), fontNormal, DARKGRAY);
    DrawText("F: Muta sageata curenta in capatul arcului. Ai voie odata per tura", textX, textY + static_cast<int>(410 * scale), fontNormal, DARKGRAY);

    float btnW = 200.0f * scale;
    float btnH = 50.0f * scale;

    float btnX = (static_cast<float>(windowWidth) - btnW) / 2.0f;
    float btnY = static_cast<float>(windowHeight) - (80.0f * scale);

    Buton back({btnX, btnY, btnW, btnH}, "BACK");

    back.OnMouseClick([&](){stare = !joc_inceput ? GameStates::StartMenu : GameStates::PauseMenu;});
    Buton::WorkInGame();
}

void GameDemo::DeseneazaGameMode() {
    float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                           static_cast<float>(windowHeight) / BASE_HEIGHT);
    float centerX = static_cast<float>(windowWidth) / 2.0f;
    float centerY = static_cast<float>(windowHeight) / 2.0f;
    float btnW = 200.0f * scale, btnH = 50.0f * scale;
    int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "ALEGE MODUL DE JOC";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 165.0f * scale), fontTitlu, DARKGRAY);

    Buton normal({centerX - btnW / 2, centerY - 105.0f * scale, btnW, btnH}, "NORMAL");
    Buton randomized({centerX - btnW / 2, centerY - 25.0f * scale, btnW, btnH}, "RANDOMIZED");
    Buton beserker({centerX - btnW / 2, centerY + 55.0f * scale, btnW, btnH}, "BESERKER");

    normal.OnMouseClick([&]() {
        game_modes_ = GameModes::Normal;
        player1 = std::make_unique<Caracter>(Arc_factory::arc_default(), BASE_CHR_SCALE, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::arc_default(), BASE_CHR_SCALE,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        player_crt = player1.get();
        AdapteazaLaFereastra();
        stare = GameStates::TuraPlayer;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    randomized.OnMouseClick([&]() {
        game_modes_ = GameModes::Randomized;
        player1 = std::make_unique<Caracter>(Arc_factory::arc_random(), BASE_CHR_SCALE, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::arc_random(), BASE_CHR_SCALE,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        player_crt = player1.get();
        AdapteazaLaFereastra();
        stare = GameStates::TuraPlayer;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    beserker.OnMouseClick([&]() {
        game_modes_ = GameModes::Beserker;
        player1 = std::make_unique<Caracter>(Arc_factory::beserker(), BASE_CHR_SCALE, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
        player2 = std::make_unique<Caracter>(Arc_factory::beserker(), BASE_CHR_SCALE,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
        p_up.SetProbability(1);
        player_crt = player1.get();
        AdapteazaLaFereastra();
        stare = GameStates::TuraPlayer;
        joc_inceput = true;
        p_up.TrySpawn();
    });
    Buton::WorkInGame();
}

void GameDemo::DeseneazaMeniuPerk() {
    const float scale = std::min(static_cast<float>(windowWidth) / BASE_WIDTH,
                                 static_cast<float>(windowHeight) / BASE_HEIGHT);
    const float centerX = static_cast<float>(windowWidth) / 2.0f;
    const float centerY = static_cast<float>(windowHeight) / 2.0f;

    const float btnW = 520.0f * scale;
    const float btnH = 66.0f * scale;
    const float gap  = 8.0f * scale;
    constexpr int   nrPerks = static_cast<int>(Perks::NrPerks);
    const float startY = centerY - (nrPerks * (btnH + gap)) / 2.0f;

    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const int fontPuncte = std::max(1, static_cast<int>(24 * scale));
    const std::string titlu = "PERK-URI";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(startY - 95.0f * scale), fontTitlu, DARKGRAY);
    const std::string pct = "Puncte: " + std::to_string(player_crt->get_puncte_afisate());
    DrawText(pct.c_str(), static_cast<int>(centerX) - MeasureText(pct.c_str(), fontPuncte) / 2,
             static_cast<int>(startY - 50.0f * scale), fontPuncte, DARKBLUE);

    std::vector<Buton> butoane;
    butoane.reserve(static_cast<size_t>(nrPerks) + 1);

    for (int i = 0; i < nrPerks; ++i) {
        const Perk& p = perks_[static_cast<size_t>(i)];
        std::string eticheta = p.Nume() + "  (" + std::to_string(p.Pret()) + ")";
        const float y = startY + static_cast<float>(i) * (btnH + gap);
        butoane.emplace_back(raylib::Rectangle{centerX - btnW / 2.0f, y, btnW, btnH},
                             std::move(eticheta), p.Descriere());
        butoane.back().OnMouseClick([this, i]() {
            const Perk& perk = perks_[static_cast<size_t>(i)];
            if (player_crt->poate_plati(perk.Pret())) {
                player_crt->plateste(perk.Pret());
                perk.AplicaEfect(player_crt);
            }
        });
    }

    const float backY = startY + static_cast<float>(nrPerks) * (btnH + gap) + gap;
    Buton inapoi({centerX - btnW / 2.0f, backY, btnW, btnH * 0.7f}, "INAPOI");
    inapoi.OnMouseClick([&]() { stare = GameStates::TuraPlayer; });

    Buton::WorkInGame();
}

void GameDemo::AdapteazaLaFereastra() const {
    Entitate::set_factor_scalare(FactorScalare());
    if (!player1 || !player2) return;
    player1->SetPosition(0.0f, static_cast<float>(windowHeight) / 2.0f);
    player2->SetPosition(static_cast<float>(windowWidth) - player1->GetHitbox().width,
                         static_cast<float>(windowHeight) / 2.0f);
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

    // Treci tura cand: o sageata a aterizat/lovit (a fost stearsa), SAU
    // jucatorul nu mai are sageti in mana SI nici una nu mai e in zbor.
    // Conditia 'sageti_zbor.empty()' e esentiala pt. beserker (1 sageata/tura):
    // altfel tura s-ar incheia instant, fara ca sageata trasa sa apuce sa zboare.
    if (std::erase_if(sageti_zbor, [](const auto& s){ return s->trebuie_stearsa(); }) > 0
        || (!player_crt->AreSageti() && sageti_zbor.empty()))
        TrecereTura = true;

    for (int i = static_cast<int>(lista_entitati.size()) - 1; i >= 0; --i) {
        const auto* b = dynamic_cast<Bloc*>(lista_entitati[i]);
        if (b != nullptr && b->TrebuieSters()) delete lista_entitati[i];
    }

    if (stare != GameStates::Intermediar) {
        player_crt->IncepeTura();
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
            c1->IncearcaMiscare(raylib::Vector2{c1->GetHitbox().x + 100.0f, c1->GetHitbox().y});
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
            c1->IncearcaMiscare(raylib::Vector2{c1->GetHitbox().x - 100.0f, c1->GetHitbox().y});
        if (IsKeyPressed(KEY_P)) {
            const float factor = FactorScalare();
            const float charH_baza = c1->GetHitbox().height / factor;
            const float inaltime_baza = (charH_baza + 10.0f) * inaltime_zid_factor;
            const float inaltime_final = inaltime_baza * factor;
            const float spawnX = c1->GetHitbox().x + offset_zid;
            const float spawnY = c1->GetHitbox().y + c1->GetHitbox().height - inaltime_final;
            new Bloc(spawnX, spawnY, latime_zid, inaltime_baza, *c1);
        }
        if (IsKeyPressed(KEY_Z)) c1->DiscardSageata();
        if (IsKeyPressed(KEY_X)) c1->Change_to_Normala();
        if (IsKeyPressed(KEY_F)) c1->MutaUltimaSageata();
        // Meniul de perk-uri se deschide doar in tura jucatorului (stare TuraPlayer).
        if (IsKeyPressed(KEY_B)) { stare = GameStates::MeniuPerk; return; }
        c1->IncearcaTragere(c2, forta_tragere, stare, sageti_zbor);
    }
}

bool GameDemo::FaraSageti(const Caracter& c1, const Caracter& c2) {
    return !c1.AreSageti() || !c2.AreSageti();
}

void GameDemo::run() {
    window.SetTargetFPS(60);
    while (!window.ShouldClose() && !close_window) {
        if (IsKeyPressed(KEY_ESCAPE) && stare != GameStates::StartMenu)
            stare = joc_inceput ? GameStates::PauseMenu : GameStates::StartMenu;

        if (window.GetWidth() != windowWidth || window.GetHeight() != windowHeight) {
            const int oldW = windowWidth, oldH = windowHeight;
            windowWidth = window.GetWidth(); windowHeight = window.GetHeight();
            AdapteazaLaFereastra();
            p_up.Reaseaza(static_cast<float>(oldW), static_cast<float>(oldH),
                          static_cast<float>(windowWidth), static_cast<float>(windowHeight));
            for (auto* e : Entitate::get_entitati()) {
                if (auto* b = dynamic_cast<Bloc*>(e)) b->Recalibreaza();
            }
        }


        if (nr_ture >= 20) p_up.SetProbability(10);
        float dt = window.GetFrameTime();
        podea = player1->GetHitbox().y + player1->GetHitbox().height + 10;
        p_up.SetSpawnArea(0.0f, static_cast<float>(windowWidth), 0.0f, 0.0f);
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
            case GameStates::MeniuStatistici: DeseneazaStats(); break;
            case GameStates::TuraPlayer: {
                if (!player1->InViata() || !player2->InViata())
                {stare = GameStates::GameOver; break;}
                starePrev = GameStates::TuraPlayer;
                stareUrm = GameStates::TuraPlayer;
                float factor = FactorScalare();
                float offset_zid = player_crt == player1.get() ?
                distanta_zid * factor + player_crt->GetHitbox().width :
                -(distanta_zid * factor + latime_zid * factor);
                Logica(player_crt, alt_player, offset_zid, dt);
                DeseneazaHUD(); break;
            }

            case GameStates::Intermediar:
                starePrev = GameStates::Intermediar;
                Logica(player_crt, alt_player, 0, dt);
                if (TrecereTura) {
                    if (player_crt->mai_are_sageti_de_tras()) {
                        // multi-shot: acelasi jucator continua sa traga
                        stare = GameStates::TuraPlayer;
                        stareUrm = GameStates::TuraPlayer;
                    } else {
                        player_crt->IncheieTura();
                        if (game_modes_ == GameModes::Beserker)
                            player_crt->PushSageata(tipSageti::Normala);
                        if (FaraSageti(*player1, *player2) ||
                            !player1->InViata() || !player2->InViata()) { stare = GameStates::GameOver; break; }
                        player_crt = alt_player;
                        player_crt->IncepeTura();
                        if (p_up.este_activ()) p_up.Consuma();
                        p_up.TrySpawn();
                        stare = stareUrm;
                        nr_ture++;
                        std:: cout << player1->get_stats() << player2->get_stats() << std::endl;
                    }
                    TrecereTura = false;
                }
                break;
            case GameStates::Controale: DeseneazaControale(); break;
            case GameStates::PauseMenu: DeseneazaPauseMenu(); break;
            case GameStates::MeniuPerk: DeseneazaMeniuPerk(); break;
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
