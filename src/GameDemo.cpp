#include "GameDemo.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "bloc.hpp"
#include "exceptii.hpp"
#include "stari.hpp"

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


Castigator GameDemo::determina_castigator() const {
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


void GameDemo::AdapteazaLaFereastra() const {
    Entitate::set_factor_scalare(FactorScalare());
    if (!player1 || !player2) return;
    player1->SetPosition(0.0f, static_cast<float>(windowHeight) / 2.0f);
    player2->SetPosition(static_cast<float>(windowWidth) - player1->GetHitbox().width,
                         static_cast<float>(windowHeight) / 2.0f);
}

void GameDemo::Logica(float dt) {
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
    if (std::erase_if(sageti_zbor, [](const auto& s){ return s->trebuie_stearsa(); }) > 0
        || (!player_crt->AreSageti() && sageti_zbor.empty()))
        TrecereTura = true;

    for (int i = static_cast<int>(lista_entitati.size()) - 1; i >= 0; --i) {
        const auto* b = dynamic_cast<Bloc*>(lista_entitati[i]);
        if (b != nullptr && b->TrebuieSters()) delete lista_entitati[i];
    }

}

bool GameDemo::FaraSageti(const Caracter& player_crt, const Caracter& alt_player) {
    return !player_crt.AreSageti() || !alt_player.AreSageti();
}

void GameDemo::RuleazaTura(float dt) {
    Logica(dt);
    if (!player1->InViata() || !player2->InViata()) { stare = GameStates::GameOver; return; }
    starePrev = GameStates::TuraPlayer;
    stareUrm = GameStates::TuraPlayer;
    const float factor = FactorScalare();
    const float offset_zid = player_crt == player1.get()
        ? distanta_zid * factor + player_crt->GetHitbox().width
        : -(distanta_zid * factor + latime_zid * factor);
    Caracter* alt_player = player_crt == player1.get() ? player2.get() : player1.get();
    player_crt->IncepeTura();
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        player_crt->IncearcaMiscare(raylib::Vector2{player_crt->GetHitbox().x + 100.0f, player_crt->GetHitbox().y});
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        player_crt->IncearcaMiscare(raylib::Vector2{player_crt->GetHitbox().x - 100.0f, player_crt->GetHitbox().y});
    if (IsKeyPressed(KEY_P)) {
        const float charH_baza = player_crt->GetHitbox().height / factor;
        const float inaltime_baza = (charH_baza + 10.0f) * inaltime_zid_factor;
        const float inaltime_final = inaltime_baza * factor;
        const float spawnX = player_crt->GetHitbox().x + offset_zid;
        const float spawnY = player_crt->GetHitbox().y + player_crt->GetHitbox().height - inaltime_final;
        new Bloc(spawnX, spawnY, latime_zid, inaltime_baza, *player_crt);
    }
    if (IsKeyPressed(KEY_Z)) player_crt->DiscardSageata();
    if (IsKeyPressed(KEY_X)) player_crt->Change_to_Normala();
    if (IsKeyPressed(KEY_F)) player_crt->MutaUltimaSageata();
    if (IsKeyPressed(KEY_B)) { stare = GameStates::MeniuPerk; return; }
    player_crt->IncearcaTragere(alt_player, forta_tragere, stare, sageti_zbor);
    DeseneazaHUD();
}

void GameDemo::RuleazaIntermediar(float dt) {
    starePrev = GameStates::Intermediar;
    Caracter* alt_player = player_crt == player1.get() ? player2.get() : player1.get();
    Logica(dt);
    if (!TrecereTura) return;

    if (player_crt->mai_are_sageti_de_tras()) {
        // multi-shot: acelasi jucator continua sa traga
        stare = GameStates::TuraPlayer;
        stareUrm = GameStates::TuraPlayer;
    } else {
        player_crt->IncheieTura();
        if (game_modes_ == GameModes::Beserker)
            player_crt->PushSageata(tipSageti::Normala);
        if (FaraSageti(*player1, *player2) ||
            !player1->InViata() || !player2->InViata()) { stare = GameStates::GameOver; return; }
        player_crt = alt_player;
        player_crt->IncepeTura();
        if (p_up.este_activ()) p_up.Consuma();
        p_up.TrySpawn();
        stare = stareUrm;
        nr_ture++;
        std::cout << player1->get_stats() << player2->get_stats() << std::endl;
    }
    TrecereTura = false;
}

void GameDemo::AlegeMod(GameModes mod) {
    game_modes_ = mod;
    player1 = std::make_unique<Caracter>(Arc_factory::creeaza(mod), BASE_CHR_SCALE, 0.0f,
                                         static_cast<float>(windowHeight) / 2.0f);
    player2 = std::make_unique<Caracter>(Arc_factory::creeaza(mod), BASE_CHR_SCALE,
                                         static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                         static_cast<float>(windowHeight) / 2.0f, 180.0f);
    if (mod == GameModes::Beserker) p_up.SetProbability(1);
    player_crt = player1.get();
    AdapteazaLaFereastra();
    stare = GameStates::TuraPlayer;
    joc_inceput = true;
    p_up.TrySpawn();
}

void GameDemo::CumparaPerk(int idx) const {
    const Perk& perk = *perks_.at(static_cast<std::size_t>(idx));
    if (player_crt->poate_plati(perk.Pret())) {
        player_crt->plateste(perk.Pret());
        perk.AplicaEfect(player_crt);
    }
}

void GameDemo::run() {
    window.SetTargetFPS(60);
    std::unique_ptr<StareJoc> stare_obj = creeaza_stare(stare);
    GameStates stare_anterioara = stare;
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

        if (stare != stare_anterioara) {
            stare_obj = creeaza_stare(stare);
            stare_anterioara = stare;
        }

        window.BeginDrawing(); window.ClearBackground(RAYWHITE);
        if (stare == GameStates::TuraPlayer || stare == GameStates::Intermediar) {
            for (auto* e : Entitate::get_entitati()) e->Draw();
            DrawLine(0, static_cast<int>(podea), windowWidth, static_cast<int>(podea), BLACK);
        }
        stare_obj->Ruleaza(*this, dt);
        window.EndDrawing();
    }
}

std::ostream& operator<<(std::ostream& os, const GameDemo& g) {
    os << "Marimea ferestrei jocului: " << g.windowWidth << "x" << g.windowHeight << std::endl;
    os << "Atributele caracterelor din joc:\nPlayer1 1:\n" << *g.player1 << "\nPlayer1 2:\n" << *g.player2;
    return os;
}
