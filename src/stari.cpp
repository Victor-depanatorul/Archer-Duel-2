#include "stari.hpp"
#include "Game.hpp"
#include "factory.hpp"
#include "exceptii.hpp"
#include "buton.hpp"
#include "caractere_variate.hpp"
#include <vector>
#include <sstream>
#include <algorithm>

// Fiecare stare apeleaza DOAR operatii publice din GameDemo (fara friend,
// fara acces direct la membri). Tranzitiile se fac din interiorul operatiilor.

namespace {
struct ItemMeniu {
    std::string text;
    std::function<void()> click;
    std::string descriere{};
};

void aseaza_grila(std::vector<Buton>& out, int W, std::vector<ItemMeniu> items,
                  float btnW, float btnH, float gap, float centerX, float centerY) {
    const int nr_butoane = static_cast<int>(items.size());
    const float pas_x = btnW + gap; // distanta orizontala intre doua butoane
    const float pas_y = btnH + gap; // inaltimea unui rand
    const float latime_utila = static_cast<float>(W) - gap;  // latimea ferestrei minus marginile
    const int butoane_pe_rand = std::clamp(static_cast<int>(latime_utila / pas_x), 1, nr_butoane);
    const int nr_randuri = (nr_butoane + butoane_pe_rand - 1) / butoane_pe_rand;
    const float inaltime_grila = static_cast<float>(nr_randuri) * pas_y;
    const float startY = centerY - inaltime_grila / 2.0f; // blocul de butoane centrat pe verticala

    out.reserve(out.size() + static_cast<size_t>(nr_butoane));
    for (int i = 0; i < nr_butoane; ++i) {
        const int rand = i / butoane_pe_rand;
        const int col = i % butoane_pe_rand;
        const int butoane_in_rand = std::min(butoane_pe_rand, nr_butoane - rand * butoane_pe_rand);
        const float latime_rand = static_cast<float>(butoane_in_rand) * btnW
                                + static_cast<float>(butoane_in_rand - 1) * gap;
        const float x = centerX - latime_rand / 2.0f + static_cast<float>(col) * pas_x;  // rand centrat
        const float y = startY + static_cast<float>(rand) * pas_y;
        out.emplace_back(raylib::Rectangle{x, y, btnW, btnH}, std::move(items[i].text), std::move(items[i].descriere));
        out.back().OnMouseClick(items[i].click);
    }
}

std::vector<std::string> imparte_pe_latime(const std::string& text, int font,
                                           int latime_prim, int latime_cont) {
    std::vector<std::string> segmente;
    std::istringstream iss(text);
    std::string cuvant, linie;
    while (iss >> cuvant) {
        std::string test = linie;
        if (!test.empty()) test += ' ';
        test += cuvant;
        const int limita = segmente.empty() ? latime_prim : latime_cont;
        if (!linie.empty() && MeasureText(test.c_str(), font) > limita) {
            segmente.push_back(linie);
            linie = cuvant;
        } else {
            linie = std::move(test);
        }
    }
    if (!linie.empty()) segmente.push_back(linie);
    return segmente;
}

// Ecran de text paginat (titlu + linii cu wrap, centrate) cu butoane BACK/PREV/NEXT.
// Folosit de meniurile de Controale si Explicatii. `pagina` e tinuta de stare.
void ecran_text(Game& g, int& pagina, const std::string& titlu,
                const std::vector<std::string>& texte) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime(), H = g.inaltime();
    const int fontTitlu = std::max(1, static_cast<int>(30 * scale));
    const int fontNormal = std::max(1, static_cast<int>(20 * scale));

    DrawText(titlu.c_str(), W / 2 - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(40 * scale), fontTitlu, DARKGRAY);

    const int latime_prim = static_cast<int>(static_cast<float>(W) * 0.85f);
    const int latime_cont = latime_prim - static_cast<int>(28 * scale);
    const int lineH = std::max(1, static_cast<int>(30 * scale));

    const float btnW = 180.0f * scale, btnH = 48.0f * scale, gap = 16.0f * scale;
    const float btnY = static_cast<float>(H) - 70.0f * scale;
    const int areaTop = static_cast<int>(100 * scale);
    const int areaBottom = static_cast<int>(btnY) - static_cast<int>(30 * scale);
    const int zona_text = std::max(lineH, areaBottom - areaTop);
    const int linii_pe_pagina = std::max(1, zona_text / lineH);

    std::vector<std::vector<std::string>> blocuri;
    blocuri.reserve(texte.size());
    for (const std::string& t : texte)
        blocuri.push_back(imparte_pe_latime(t, fontNormal, latime_prim, latime_cont));

    std::vector<int> start_pagina{0};
    int linii = 0;
    for (int i = 0; i < static_cast<int>(blocuri.size()); ++i) {
        const int h = static_cast<int>(blocuri[i].size());
        if (linii > 0 && linii + h > linii_pe_pagina) { start_pagina.push_back(i); linii = 0; }
        linii += h;
    }
    const int nr_pagini = static_cast<int>(start_pagina.size());
    pagina = std::clamp(pagina, 0, nr_pagini - 1);
    const int de_la = start_pagina[pagina];
    const int pana_la = (pagina + 1 < nr_pagini) ? start_pagina[pagina + 1]
                                                 : static_cast<int>(blocuri.size());

    int total_linii = 0;
    for (int i = de_la; i < pana_la; ++i) total_linii += static_cast<int>(blocuri[i].size());
    int y = areaTop + std::max(0, (zona_text - total_linii * lineH) / 2);

    for (int i = de_la; i < pana_la; ++i) {
        const std::vector<std::string>& seg = blocuri[i];
        for (size_t j = 0; j < seg.size(); ++j) {
            std::string linie;
            Color culoare;
            if (j == 0) {
                linie = seg[j];
                culoare = DARKGRAY;
            } else {
                linie = "- ";
                linie += seg[j];
                culoare = GRAY;
            }
            const int x = W / 2 - MeasureText(linie.c_str(), fontNormal) / 2;
            DrawText(linie.c_str(), x, y, fontNormal, culoare);
            y += lineH;
        }
    }

    if (nr_pagini > 1) {
        std::string ind = "Pagina ";
        ind += std::to_string(pagina + 1);
        ind += '/';
        ind += std::to_string(nr_pagini);
        DrawText(ind.c_str(), W / 2 - MeasureText(ind.c_str(), fontNormal) / 2,
                 static_cast<int>(btnY) - lineH, fontNormal, DARKBLUE);
    }

    std::vector<Buton> butoane;
    butoane.reserve(3);
    const float centerX = static_cast<float>(W) / 2.0f;
    butoane.emplace_back(raylib::Rectangle{centerX - btnW / 2.0f, btnY, btnW, btnH}, "BACK");
    butoane.back().OnMouseClick([&]{ g.SchimbaStare(g.joc_a_inceput() ? GameStates::PauseMenu : GameStates::StartMenu); });
    if (pagina > 0) {
        butoane.emplace_back(raylib::Rectangle{centerX - btnW * 1.5f - gap, btnY, btnW, btnH}, "PREV");
        butoane.back().OnMouseClick([&pagina]{ --pagina; });
    }
    if (pagina + 1 < nr_pagini) {
        butoane.emplace_back(raylib::Rectangle{centerX + btnW / 2.0f + gap, btnY, btnW, btnH}, "NEXT");
        butoane.back().OnMouseClick([&pagina]{ ++pagina; });
    }
    Buton::WorkInGame();
}
}

void StareStartMenu::Ruleaza(Game& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime();
    const float centerX = static_cast<float>(W) / 2.0f;
    const float centerY = static_cast<float>(g.inaltime()) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale, gap = 20.0f * scale;
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "ARCHER DUEL";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 165.0f * scale), fontTitlu, DARKGRAY);

    std::vector<Buton> butoane;
    aseaza_grila(butoane, W, {
        {"START GAME", [&]{ g.SchimbaStare(GameStates::MeniuGameModes); }},
        {"CONTROALE",  [&]{ g.SchimbaStare(GameStates::Controale); }},
        {"EXPLICATII", [&]{ g.SchimbaStare(GameStates::Explicatii); }},
        {"EXIT GAME",  [&]{ g.Inchide(); }},
    }, btnW, btnH, gap, centerX, centerY);
    Buton::WorkInGame();
}

void StareControale::Ruleaza(Game& g, float) {
    static const std::vector<std::string> controale = {
        "ESCAPE: Deschide meniul principal.",
        "LEFT CLICK: Pregateste tragerea cu arcul.",
        "Mouse Wheel: Modifica forta de tragere (sus o creste, jos o scade).",
        "C: Anuleaza actiunea de tragere.",
        "P: Spawneaza un perete defensiv (cooldown o tura).",
        "Z: Arunca sageata curenta din arc.",
        "X: Schimba sageata curenta intr-una normala. Ai voie o data per tura.",
        "F: Muta sageata curenta in capatul arcului. Ai voie o data per tura.",
        "B: Deschide meniul de perk-uri.",
        "Asasin - E: lovitura de aproape mai puternica (cooldown o tura).",
        "Mage - E: heal. R: sageata otravitoare. T: rezistenta (consuma mana).",
        "Reinforcer - E: rezistenta la damage o tura (cooldown o tura).",
    };
    ecran_text(g, pagina, "CONTROALE JOC", controale);
}

void StareExplicatii::Ruleaza(Game& g, float) {
    static const std::vector<std::string> explicatii = {
        "Obiectiv: adu HP-ul adversarului la 0. Daca se termina sagetile fara KO, castiga cine mai are sageti, iar la egalitate cine are mai mult HP.",
        "Power up-uri: dreptunghiurile care plutesc deasupra terenului. Le colectezi tragand cu o sageata in ele; culoarea arata tipul.",
        "ARROW: primesti o sageata. MULTISHOT: tragi cu 2-4 sageti tura urmatoare. DOUBLE MOVE: te poti misca de doua ori.",
        "HUD: in colturi vezi HP, Puncte, Clasa si Urmeaza (tipul si culoarea sagetii urmatoare). Mage afiseaza in plus Mana.",
        "Puncte: le primesti din damage si acuratete; le cheltui pe perk-uri din meniul deschis cu tasta B.",
    };
    ecran_text(g, pagina, "EXPLICATII", explicatii);
}

void StareTuraPlayer::Ruleaza(Game& g, float dt) {
    g.RuleazaTura(dt);
}

void StareIntermediar::Ruleaza(Game& g, float dt) {
    g.RuleazaIntermediar(dt);
}

void StarePauseMenu::Ruleaza(Game& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime();
    const float centerX = static_cast<float>(W) / 2.0f;
    const float centerY = static_cast<float>(g.inaltime()) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale, gap = 20.0f * scale;
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const std::string titlu = "PAUZA";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(centerY - 195.0f * scale), fontTitlu, DARKGRAY);

    std::vector<Buton> butoane;
    aseaza_grila(butoane, W, {
        {"RESUME",    [&]{ g.Reia(); }},
        {"RESTART",   [&]{ g.Restart(); }},
        {"CONTROALE", [&]{ g.SchimbaStare(GameStates::Controale); }},
        {"EXPLICATII", [&]{ g.SchimbaStare(GameStates::Explicatii); }},
        {"EXIT GAME", [&]{ g.Inchide(); }},
    }, btnW, btnH, gap, centerX, centerY);

    Buton::WorkInGame();
}

void StareGameOver::Ruleaza(Game& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime();
    const float centerX = static_cast<float>(W) / 2.0f;
    const float centerY = static_cast<float>(g.inaltime()) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale, gap = 20.0f * scale;
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    const int fontMesaj = std::max(1, static_cast<int>(30 * scale));
    const std::string titlu = "GAME OVER";
    DrawText(titlu.c_str(), static_cast<int>(centerX) - MeasureText(titlu.c_str(), fontTitlu) / 2, static_cast<int>(centerY - 150.0f * scale), fontTitlu, RED);
    std::string mesaj;
    switch (g.determina_castigator()) {
        case Castigator::Player1:   mesaj = "PLAYER 1 A CASTIGAT"; break;
        case Castigator::Player2:   mesaj = "PLAYER 2 A CASTIGAT"; break;
        case Castigator::Egalitate: mesaj = "EGALITATE"; break;
    }
    DrawText(mesaj.c_str(), static_cast<int>(centerX) - MeasureText(mesaj.c_str(), fontMesaj) / 2, static_cast<int>(centerY - 100.0f * scale), fontMesaj, BLACK);

    std::vector<Buton> butoane;
    aseaza_grila(butoane, W, {
        {"PLAY AGAIN",       [&]{ g.PlayAgain(); }},
        {"VREAU STATISTICI", [&]{ g.SchimbaStare(GameStates::MeniuStatistici); }},
        {"EXIT GAME",        [&]{ g.Inchide(); }},
    }, btnW, btnH, gap, centerX, centerY + 40.0f * scale);
    Buton::WorkInGame();
}

void StareMeniuGameModes::Ruleaza(Game& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime();
    const float cx = static_cast<float>(W) / 2.0f, cy = static_cast<float>(g.inaltime()) / 2.0f;
    const float btnW = 200.0f * scale, btnH = 50.0f * scale, gap = 20.0f * scale;
    const std::string titlu = "ALEGE MODUL DE JOC";
    const int fontTitlu = std::max(1, static_cast<int>(40 * scale));
    DrawText(titlu.c_str(), static_cast<int>(cx) - MeasureText(titlu.c_str(), fontTitlu) / 2,
             static_cast<int>(cy - 165.0f * scale), fontTitlu, DARKGRAY);

    std::vector<Buton> butoane;
    aseaza_grila(butoane, W, {
        {"NORMAL",     [&]{ g.AlegeMod(GameModes::Normal); }},
        {"RANDOMIZED", [&]{ g.AlegeMod(GameModes::Randomized); }},
        {"BESERKER",   [&]{ g.AlegeMod(GameModes::Beserker); }},
    }, btnW, btnH, gap, cx, cy);
    Buton::WorkInGame();
}
void StareMeniuStatistici::Ruleaza(Game& g, float) {
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
void StareMeniuPerk::Ruleaza(Game& g, float) {
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

void StareAlegeCaracter::Ruleaza(Game& g, float) {
    const float scale = g.ScalaMeniu();
    const int W = g.latime(), H = g.inaltime();
    const float centerX = static_cast<float>(W) / 2.0f;
    const auto& catalog = Caracter_factory::catalog();
    constexpr int n = tipCaracter::NrCaractere;

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
        const int maxW = static_cast<int>(static_cast<float>(W) * 0.9f);
        const int lineH = fontD + static_cast<int>(6 * scale);

        std::vector<std::string> linii;
        std::string linie_curenta;
        std::istringstream iss(d);
        std::string cuvant;
        while (iss >> cuvant) {
            std::string test = linie_curenta;
            if (!test.empty()) test += ' ';
            test += cuvant;
            if (!linie_curenta.empty() && MeasureText(test.c_str(), fontD) > maxW) {
                linii.push_back(linie_curenta);
                linie_curenta = cuvant;
            } else {
                linie_curenta = std::move(test);
            }
        }
        if (!linie_curenta.empty()) linii.push_back(linie_curenta);

        int y = static_cast<int>(static_cast<float>(H) * 0.82f);
        for (const std::string& l : linii) {
            DrawText(l.c_str(), static_cast<int>(centerX) - MeasureText(l.c_str(), fontD) / 2, y, fontD, DARKBLUE);
            y += lineH;
        }
    }

    Buton::WorkInGame();
}

std::unique_ptr<StareJoc> creeaza_stare(GameStates tip) {
    static const Factory<GameStates, StareJoc, GameStates::NrStates> fabrica = [] {
        Factory<GameStates, StareJoc, GameStates::NrStates> f;
        f.inregistreaza(GameStates::StartMenu,       [] { return std::make_unique<StareStartMenu>(); });
        f.inregistreaza(GameStates::Controale,       [] { return std::make_unique<StareControale>(); });
        f.inregistreaza(GameStates::TuraPlayer,      [] { return std::make_unique<StareTuraPlayer>(); });
        f.inregistreaza(GameStates::Intermediar,     [] { return std::make_unique<StareIntermediar>(); });
        f.inregistreaza(GameStates::PauseMenu,       [] { return std::make_unique<StarePauseMenu>(); });
        f.inregistreaza(GameStates::GameOver,        [] { return std::make_unique<StareGameOver>(); });
        f.inregistreaza(GameStates::MeniuGameModes,  [] { return std::make_unique<StareMeniuGameModes>(); });
        f.inregistreaza(GameStates::MeniuStatistici, [] { return std::make_unique<StareMeniuStatistici>(); });
        f.inregistreaza(GameStates::MeniuPerk,       [] { return std::make_unique<StareMeniuPerk>(); });
        f.inregistreaza(GameStates::AlegeCaracter,   [] { return std::make_unique<StareAlegeCaracter>(); });
        f.inregistreaza(GameStates::Explicatii,      [] { return std::make_unique<StareExplicatii>(); });
        return f;
    }();

    auto s = fabrica.creeaza(tip);
    if (!s) throw eroare_stare(static_cast<int>(tip));
    return s;
}
