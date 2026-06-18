#include "stari.hpp"
#include "GameDemo.hpp"
#include "factory.hpp"
#include "exceptii.hpp"
#include "buton.hpp"
#include "caractere_variate.hpp"
#include <vector>

// Fiecare stare apeleaza DOAR operatii publice din GameDemo (fara friend,
// fara acces direct la membri). Tranzitiile se fac din interiorul operatiilor.
void StareStartMenu::Ruleaza(GameDemo& g, float) {
    float scale = g.ScalaMeniu();
    float centerX = static_cast<float>(g.latime()) / 2.0f;
    float centerY = static_cast<float>(g.inaltime()) / 2.0f;
    float btnW = 200.0f * scale, btnH = 50.0f * scale;
    int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "ARCHER DUEL";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 165.0f * scale), fontTitlu, DARKGRAY);

    Buton start({centerX - btnW / 2, centerY - 105.0f * scale, btnW, btnH}, "START GAME");
    Buton controale({centerX - btnW / 2, centerY - 25.0f * scale, btnW, btnH}, "CONTROALE");
    Buton exit({centerX - btnW / 2, centerY + 55.0f * scale, btnW, btnH}, "EXIT GAME");
    start.OnMouseClick([&]() { g.SchimbaStare(GameStates::MeniuGameModes);});
    controale.OnMouseClick([&](){g.SchimbaStare(GameStates::Controale);});
    exit.OnMouseClick([&](){g.Inchide();});
    Buton::WorkInGame();
}

void StareControale::Ruleaza(GameDemo& g, float) {
    float scale = g.ScalaMeniu();

    int fontTitlu = std::max(1, static_cast<int>(30 * scale));
    int fontNormal = std::max(1, static_cast<int>(20 * scale));
    int fontMic = std::max(1, static_cast<int>(16 * scale));

    const int windowWidth = g.latime(), windowHeight = g.inaltime();

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
    DrawText("B: Deschide meniul de perk-uri.", textX, textY + static_cast<int>(465 * scale), fontNormal, DARKGRAY);

    float btnW = 200.0f * scale;
    float btnH = 50.0f * scale;

    float btnX = (static_cast<float>(windowWidth) - btnW) / 2.0f;
    float btnY = static_cast<float>(windowHeight) - (80.0f * scale);

    Buton back({btnX, btnY, btnW, btnH}, "BACK");

    back.OnMouseClick([&]{ g.SchimbaStare(g.joc_a_inceput() ? GameStates::PauseMenu : GameStates::StartMenu); });
    Buton::WorkInGame();
}

void StareTuraPlayer::Ruleaza(GameDemo& g, float dt) {
    g.RuleazaTura(dt);
}

void StareIntermediar::Ruleaza(GameDemo& g, float dt) {
    g.RuleazaIntermediar(dt);
}

void StarePauseMenu::Ruleaza(GameDemo& g, float) {
    float scale = g.ScalaMeniu();
    const int windowWidth = g.latime(), windowHeight = g.inaltime();
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

    resume.OnMouseClick([&](){g.Reia();});
    restart.OnMouseClick([&]() {g.Restart();});
    controale.OnMouseClick([&]() {g.SchimbaStare(GameStates::Controale);});
    exit.OnMouseClick([&](){g.Inchide();});

    Buton::WorkInGame();
}

void StareGameOver::Ruleaza(GameDemo& g, float) {
    const float scale = g.ScalaMeniu();
    const int windowWidth = g.latime(), windowHeight = g.inaltime();
    const float centerX = static_cast<float>(windowWidth) / 2.0f;
    const float centerY = static_cast<float>(windowHeight) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale;
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const int fontMesaj = std::max(1, static_cast<int>(30 * scale));
    const std::string titlu = "GAME OVER";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2, static_cast<int>(centerY - 100.0f * scale), fontTitlu, RED);
    std::string mesaj;
    switch (g.determina_castigator()) {
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

    play_again.OnMouseClick([&](){g.PlayAgain();});
    exit.OnMouseClick([&](){g.Inchide();});
    stats.OnMouseClick([&](){g.SchimbaStare(MeniuStatistici);});
    Buton::WorkInGame();
}

void StareMeniuGameModes::Ruleaza(GameDemo& g, float) {
    float scale = g.ScalaMeniu();
    float cx = static_cast<float>(g.latime()) / 2.0f, cy = static_cast<float>(g.inaltime()) / 2.0f;
    float btnW = 200 * scale, btnH = 50 * scale;
    const std::string titlu = "ALEGE MODUL DE JOC";
    int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    DrawText(titlu.c_str(), static_cast<int>(cx) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(cy - 165.0f * scale), fontTitlu, DARKGRAY);
    Buton normal  ({cx - btnW/2, cy - 105*scale, btnW, btnH}, "NORMAL");
    Buton random  ({cx - btnW/2, cy -  25*scale, btnW, btnH}, "RANDOMIZED");
    Buton beserker({cx - btnW/2, cy +  55*scale, btnW, btnH}, "BESERKER");
    normal.OnMouseClick  ([&]{ g.AlegeMod(GameModes::Normal); });
    random.OnMouseClick  ([&]{ g.AlegeMod(GameModes::Randomized); });
    beserker.OnMouseClick([&]{ g.AlegeMod(GameModes::Beserker); });
    Buton::WorkInGame();
}
void StareMeniuStatistici::Ruleaza(GameDemo& g, float) {
    const float scale = g.ScalaMeniu() * 1.3f;
    const int windowWidth = g.latime(), windowHeight = g.inaltime();
    const int centerX = windowWidth / 2;
    const int centerY = windowHeight / 2;

    const int colW = static_cast<int>(200 * scale);
    const int colGap = static_cast<int>(60 * scale);
    const int statsY = centerY - static_cast<int>(140 * scale);
    const int leftX = centerX - colGap / 2 - colW;
    const int rightX = centerX + colGap / 2;

    g.jucator1_()->get_stats().Deseneaza(leftX, statsY, BLACK, "Player 1:", scale);
    g.jucator2_()->get_stats().Deseneaza(rightX, statsY, BLACK, "Player 2:", scale);

    const float btnW = 200.0f * scale, btnH = 50.0f * scale;
    const float btnX = static_cast<float>(centerX) - btnW / 2.0f;
    const float btnY = static_cast<float>(statsY) + 6 * 25 * scale + 30.0f * scale;
    Buton back({btnX, btnY, btnW, btnH}, "GO BACK");
    back.OnMouseClick([&](){g.SchimbaStare(GameStates::GameOver);});
    Buton::WorkInGame();
}
void StareMeniuPerk::Ruleaza(GameDemo& g, float) {
    const float scale = g.ScalaMeniu();
    const int windowWidth = g.latime(), windowHeight = g.inaltime();
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
    const std::string pct = "Puncte: " + std::to_string(g.jucator_curent()->get_puncte_afisate());
    DrawText(pct.c_str(), static_cast<int>(centerX) - MeasureText(pct.c_str(), fontPuncte) / 2,
             static_cast<int>(startY - 50.0f * scale), fontPuncte, DARKBLUE);

    std::vector<Buton> butoane;
    butoane.reserve(static_cast<size_t>(nrPerks) + 1);

    for (int i = 0; i < nrPerks; ++i) {
        const Perk& p = *g.perks()[static_cast<size_t>(i)];
        std::string eticheta = p.Nume() + "  (" + std::to_string(p.Pret()) + ")";
        const float y = startY + static_cast<float>(i) * (btnH + gap);
        butoane.emplace_back(raylib::Rectangle{centerX - btnW / 2.0f, y, btnW, btnH},
                             std::move(eticheta), p.Descriere());
        butoane.back().OnMouseClick([&g, i]() {
            g.CumparaPerk(i);
        });
    }

    const float backY = startY + static_cast<float>(nrPerks) * (btnH + gap) + gap;
    Buton inapoi({centerX - btnW / 2.0f, backY, btnW, btnH * 0.7f}, "INAPOI");
    inapoi.OnMouseClick([&]() {g.SchimbaStare(GameStates::TuraPlayer);});

    Buton::WorkInGame();
}

void StareAlegeCaracter::Ruleaza(GameDemo& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime(), H = g.inaltime();
    const float centerX = static_cast<float>(W) / 2.0f;
    const auto& catalog = Caracter_factory::catalog();
    constexpr int n = static_cast<int>(NrCaractere);

    const float btnW = 220.0f * scale, btnH = 56.0f * scale, gap = 12.0f * scale;

    const int fontTitlu = std::max(1, static_cast<int>(36 * scale));
    const std::string titlu = std::string("JUCATOR ") + (jucator == 0 ? "1" : "2") + " - ALEGE CLASA";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(static_cast<float>(H) * 0.12f), fontTitlu, DARKGRAY);

    int pe_rand = std::max(1, static_cast<int>((static_cast<float>(W) - gap) / (btnW + gap)));
    pe_rand = std::min(pe_rand, n);
    const int randuri = (n + pe_rand - 1) / pe_rand;
    const float startY = static_cast<float>(H) / 2.0f - static_cast<float>(randuri) * (btnH + gap) / 2.0f;

    std::vector<Buton> butoane;
    butoane.reserve(static_cast<size_t>(n) + 1);

    for (int i = 0; i < n; ++i) {
        const int rand = i / pe_rand;
        const int col = i % pe_rand;
        const int in_rand = std::min(pe_rand, n - rand * pe_rand);
        const float rowW = static_cast<float>(in_rand) * btnW + static_cast<float>(in_rand - 1) * gap;
        const float x = centerX - rowW / 2.0f + static_cast<float>(col) * (btnW + gap);
        const float y = startY + static_cast<float>(rand) * (btnH + gap);

        if (i == previzualizat) {
            butoane.emplace_back(raylib::Rectangle{x, y, btnW, btnH}, "SELECTEAZA");
            butoane.back().OnMouseClick([&g, this]() {
                const auto ales = static_cast<tipCaracter>(previzualizat);
                if (jucator == 0) { ales_p1 = ales; jucator = 1; previzualizat = -1; detalii = false; }
                else g.IncepeMeci(ales_p1, ales);
            });
            butoane.emplace_back(raylib::Rectangle{x, y + btnH + gap, btnW, btnH * 0.7f}, "DETALII");
            butoane.back().OnMouseClick([this]() { detalii = !detalii; });
        } else {
            butoane.emplace_back(raylib::Rectangle{x, y, btnW, btnH}, std::string(catalog[static_cast<size_t>(i)].nume));
            butoane.back().OnMouseClick([this, i]() { previzualizat = i; detalii = false; });
        }
    }

    if (detalii && previzualizat >= 0) {
        const std::string& d = catalog[static_cast<size_t>(previzualizat)].descriere;
        const int fontD = std::max(1, static_cast<int>(18 * scale));
        DrawText(d.c_str(), static_cast<int>(centerX) - MeasureText(d.c_str(), fontD) / 2,
                 static_cast<int>(static_cast<float>(H) * 0.82f), fontD, DARKBLUE);
    }

    Buton::WorkInGame();
}

std::unique_ptr<StareJoc> creeaza_stare(GameStates tip) {
    static const Factory<GameStates, StareJoc, NrStates> fabrica = [] {
        Factory<GameStates, StareJoc, NrStates> f;
        f.inregistreaza(StartMenu,       [] { return std::make_unique<StareStartMenu>(); });
        f.inregistreaza(Controale,       [] { return std::make_unique<StareControale>(); });
        f.inregistreaza(TuraPlayer,      [] { return std::make_unique<StareTuraPlayer>(); });
        f.inregistreaza(Intermediar,     [] { return std::make_unique<StareIntermediar>(); });
        f.inregistreaza(PauseMenu,       [] { return std::make_unique<StarePauseMenu>(); });
        f.inregistreaza(GameOver,        [] { return std::make_unique<StareGameOver>(); });
        f.inregistreaza(MeniuGameModes,  [] { return std::make_unique<StareMeniuGameModes>(); });
        f.inregistreaza(MeniuStatistici, [] { return std::make_unique<StareMeniuStatistici>(); });
        f.inregistreaza(MeniuPerk,       [] { return std::make_unique<StareMeniuPerk>(); });
        f.inregistreaza(AlegeCaracter,   [] { return std::make_unique<StareAlegeCaracter>(); });
        return f;
    }();

    auto s = fabrica.creeaza(tip);
    if (!s) throw eroare_stare(static_cast<int>(tip));
    return s;
}
