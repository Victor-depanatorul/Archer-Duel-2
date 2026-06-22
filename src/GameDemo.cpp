#include "GameDemo.hpp"
#include "sageata.hpp"
#include "arc.hpp"
#include "exceptii.hpp"
#include "stari.hpp"
#include "caractere_variate.hpp"

GameDemo::GameDemo() : p_up(0, 0, 0 ,0),
window(windowWidth, windowHeight, "Archer Duel", FLAG_WINDOW_RESIZABLE) {
    SetExitKey(KEY_NULL);
    window.SetMinSize(400, 300);
    player1 = std::make_unique<Caracter>(Arc_factory::creeaza(GameModes::Normal), BASE_CHR_SCALE, 0.0f,
                                     static_cast<float>(windowHeight) / 2.0f);
    player2 = std::make_unique<Caracter>(Arc_factory::creeaza(GameModes::Normal), BASE_CHR_SCALE,
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
        if (lista[i]->este_obstacol())
            delete lista[i];
    }

    game_modes_ = GameModes::Normal;

    stare = GameStates::StartMenu;
    stareUrm = GameStates::TuraPlayer;
    starePrev = GameStates::TuraPlayer;
}

void GameDemo::DeseneazaHUD() const {
    constexpr int fontSize = 20, fontInfo = fontSize - 3, padding = 20;
    const bool p1_curent = (player_crt == player1.get());

    auto deseneaza = [&](const Caracter& c, bool stanga, bool curent) {
        auto x = [&](const std::string& s, int font) {
            return stanga ? padding : windowWidth - padding - MeasureText(s.c_str(), font);
        };
        int linie = padding;
        if (curent) {
            const std::string tura = stanga ? "TURA PLAYER 1" : "TURA PLAYER 2";
            DrawText(tura.c_str(), x(tura, fontSize - 5), linie, fontSize - 5, RED);
            linie += fontSize;
        }
        for (const LinieHud& l : c.info_hud()) {
            DrawText(l.first.c_str(), x(l.first, fontInfo), linie, fontInfo, l.second);
            linie += fontInfo + 4;
        }
    };

    deseneaza(*player1, true,  p1_curent);
    deseneaza(*player2, false, !p1_curent);
}


Castigator GameDemo::determina_castigator() const {
    return Caracter::determina_castigator(*player1, *player2);
}


void GameDemo::AdapteazaLaFereastra() const {
    Entitate::set_factor_scalare(FactorScalare());
    if (!player1 || !player2) return;
    const float sol = static_cast<float>(windowHeight) / 2.0f + player1->inaltime_de_baza();
    player1->SetPosition(0.0f, sol - player1->GetHitbox().height);
    player2->SetPosition(static_cast<float>(windowWidth) - player2->GetHitbox().width,
                         sol - player2->GetHitbox().height);
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

    for (int i = static_cast<int>(lista_entitati.size()) - 1; i >= 0; --i)
        if (lista_entitati[i]->TrebuieSters()) delete lista_entitati[i];

}

bool GameDemo::FaraSageti(const Caracter& player_crt, const Caracter& alt_player) {
    return !player_crt.AreSageti() || !alt_player.AreSageti();
}

void GameDemo::RuleazaTura(float dt) {
    Logica(dt);
    if (!player1->InViata() || !player2->InViata()) { stare = GameStates::GameOver; return; }
    starePrev = GameStates::TuraPlayer;
    stareUrm = GameStates::TuraPlayer;
    if (IsKeyPressed(KEY_B)) { stare = GameStates::MeniuPerk; return; }
    const float factor = FactorScalare();
    const Caracter* alt_player = player_crt == player1.get() ? player2.get() : player1.get();
    player_crt->IncearcaActiuni(alt_player, forta_tragere, stare, sageti_zbor,
                                factor, player_crt == player1.get(),
                                distanta_zid, latime_zid, inaltime_zid_factor);
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
    if (mod == GameModes::Beserker) p_up.SetProbability(1);
    stare = GameStates::AlegeCaracter;
}

void GameDemo::IncepeMeci(tipCaracter t1, tipCaracter t2) {
    player1 = Caracter_factory::creeaza(t1, Arc_factory::creeaza(game_modes_), BASE_CHR_SCALE, 0.0f,
                                        static_cast<float>(windowHeight) / 2.0f, 0.0f);
    player2 = Caracter_factory::creeaza(t2, Arc_factory::creeaza(game_modes_), BASE_CHR_SCALE,
                                        static_cast<float>(windowWidth) - player1->GetHitbox().width,
                                        static_cast<float>(windowHeight) / 2.0f, 180.0f);
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
            for (auto* e : Entitate::get_entitati())
                e->Recalibreaza();
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
