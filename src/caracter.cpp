#include "caracter.hpp"
#include "PowerUp.hpp"
#include "exceptii.hpp"
#include "bloc.hpp"

#include <cmath>
#include <utility>

namespace {
    const char* verifica_textura(const char* path) {
        if (!FileExists(path))
            throw eroare_textura(path);
        return path;
    }
}

// Constructori
Caracter::Caracter(float scale, float posX, float posY, float rotation)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation),
    textura(verifica_textura(path.c_str())){
    base_scale = scale;
    set_dimensiuni_baza(static_cast<float>(textura.GetWidth()),
                        static_cast<float>(textura.GetHeight()));
}

Caracter::Caracter(Arc arc, float scale, float posX, float posY, float rotation)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation),
    arc(std::move(arc)),  textura(verifica_textura(path.c_str())){
    base_scale = scale;
    set_dimensiuni_baza(static_cast<float>(textura.GetWidth()),
                        static_cast<float>(textura.GetHeight()));
}



void Caracter::Change_to_Normala() {
    if (a_schimbat_normala) return;
    arc.PopSageata(); arc.PushSageata(tipSageti::Normala);
    a_schimbat_normala = true;
}

void Caracter::MutaUltimaSageata() {
    if (a_mutat_sageata) return;
    arc.MutaUltimaSageata();
    a_mutat_sageata = true;
}

float Caracter::vulnerabilitate(const Sageata&) const { return 1.0f; }
float Caracter::multiplicator_damage_primit() const { return 1.0f; }

float Caracter::inaltime_de_baza() const { return inaltime_baza * base_scale * get_factor_scalare(); }
std::string Caracter::nume_clasa() const { return "None"; }

std::vector<LinieHud> Caracter::info_hud() const {
    return {
        {"HP: " + std::to_string(static_cast<int>(hp)), GREEN},
        {"Puncte: " + std::to_string(get_puncte_afisate()), DARKGRAY},
        {"Clasa: " + nume_clasa(), DARKGRAY},
        {"Urmeaza: " + TipUrmatoareaSageata(), CuloareUrmatoareaSageata()}
    };
}

bool Caracter::in_miscare() const { return se_misca; }
Statistici Caracter::get_stats() const { return stats; }

void Caracter::stats_powerup() { stats.inregistreaza_powerup(); }
void Caracter::stats_nimerita() { stats.inregistreaza_nimerita(); }
void Caracter::IncepeTura() { tura_activa = true; }
void Caracter::PrimesteMultiShot(int n) { sageti_de_tras_urm = n; }
void Caracter::PrimesteDoubleMove() { miscari_ramase_urm = 2; }
void Caracter::Heal(float cantitate) { hp += cantitate; }

void Caracter::SetDmgMultiplier(float mult, bool permanent, int runde) {
    dmg_multiplier += mult;
    runde_dmg_multiplier = permanent ? -1 : runde + 1;
}

void Caracter::SetArmorMultiplier(float mult, bool permanent, int runde) {
    armor_multiplier = mult;
    runde_armor_multiplier = permanent ? -1 : runde + 1;
}

float Caracter::get_dmg_multiplier() const { return dmg_multiplier; }
bool Caracter::mai_are_sageti_de_tras() const { return sageti_de_tras > 0 && AreSageti(); }
void Caracter::DiscardSageata() { arc.PopSageata(); }

void Caracter::add_puncte(float multiplier) { puncte += stats.acuratete() * multiplier; }
int Caracter::get_puncte_afisate() const { return static_cast<int>(puncte * 100); }
bool Caracter::poate_plati(int cost) const { return get_puncte_afisate() >= cost; }
void Caracter::plateste(int cost) { puncte -= static_cast<float>(cost) / 100.0f; }

int Caracter::durabilitate_zid() const { return 1; }

Castigator Caracter::determina_castigator(const Caracter& a, const Caracter& b) {
    const bool a_viu = a.InViata();
    const bool b_viu = b.InViata();
    if (a_viu && !b_viu) return Castigator::Player1;
    if (b_viu && !a_viu) return Castigator::Player2;

    const bool a_sageti = a.AreSageti();
    const bool b_sageti = b.AreSageti();
    if (a_sageti && !b_sageti) return Castigator::Player1;
    if (b_sageti && !a_sageti) return Castigator::Player2;

    if (a.hp > b.hp) return Castigator::Player1;
    if (b.hp > a.hp) return Castigator::Player2;
    return Castigator::Egalitate;
}

std::string Caracter::TipUrmatoareaSageata() const {
    auto* s = arc.VeziUrmatoarea();
    if (s != nullptr)
        return s->nume();
    return "Epuizat";
}

Color Caracter::CuloareUrmatoareaSageata() const {
    auto* s = arc.VeziUrmatoarea();
    return s != nullptr ? s->get_color() : DARKGRAY;
}

bool Caracter::InViata() const { return hp > 0; }
bool Caracter::AreSageti() const { return arc.AreSageti(); }

void Caracter::_draw(raylib::Vector2) {
    // Desenam la dimensiunea hitbox-ului, care e deja scalat (scale * factor_scalare).
    const float w = hitbox.width;
    const float h = hitbox.height;
    raylib::Rectangle src = {0, 0, static_cast<float>(textura.GetWidth()), static_cast<float>(textura.GetHeight())};
    raylib::Vector2 origineRotatie = { w / 2.0f, h / 2.0f };
    raylib::Rectangle dest = { hitbox.x + origineRotatie.x, hitbox.y + origineRotatie.y, w, h };
    textura.Draw(src, dest, origineRotatie, rotation);
}

void Caracter::IaDamage(const float dmg, float multiplier) {
    if (dmg >= 0.0f) stats.inregistreaza_damage(dmg * multiplier * armor_multiplier);
    const float dodge = dmg > 0.0f ? multiplicator_damage_primit() * vuln_curenta : 1.0f;
    hp -= dmg * armor_multiplier * multiplier * dodge;
}

void Caracter::IaDamageEfect(const float dmg) {
    // Damage de la efecte (otrava/burn): ignora armura.
    if (dmg >= 0.0f) stats.inregistreaza_damage(dmg);
    hp -= dmg;
}

void Caracter::AplicaOtrava(const int runde) { runde_otrava = std::min(runde_otrava + runde, 5); }

void Caracter::AplicaBurn(const int ture) { runde_burn = std::max(runde_burn, ture); }


void Caracter::UpdateBurn(float dt) {
    if (tura_activa && runde_burn > 0) {
        IaDamageEfect(burn_dps_frame * dt);
    }
}

void Caracter::OnCollision(Entitate& other) {
        Caracter* mutat = se_misca ? this : dynamic_cast<Caracter*>(&other);
        if (mutat == nullptr || !mutat->se_misca) return;
        const Entitate& obstacol = (mutat == this) ? other : static_cast<Entitate&>(*this);
        const float rad = mutat->rotatie_baza * (PI / 180.0f);
        const float distantaRespingere = obstacol.GetHitbox().width + 10.0f;
        mutat->pozitieTinta = raylib::Vector2(mutat->hitbox.x, mutat->hitbox.y);
        mutat->MoveWith(-std::cos(rad) * distantaRespingere, -std::sin(rad) * distantaRespingere);
        mutat->se_misca = false;
        mutat->IaDamage(1.0f, 1.0f);
}

void Caracter::OnCollision(Sageata &s) {
    vuln_curenta = vulnerabilitate(s);
    s.aplica_efect(*this);
    vuln_curenta = 1.0f;
}

void Caracter::IncheieTura() {
        tura_activa = false;
        if (runde_burn > 0) --runde_burn;
        if (runde_otrava > 0) { IaDamageEfect(dps_otrava); --runde_otrava; }
        if (runde_dmg_multiplier > 0 && --runde_dmg_multiplier == 0) dmg_multiplier = 1.0f;
        if (runde_armor_multiplier > 0 && --runde_armor_multiplier == 0) armor_multiplier = 1.0f;
        if (cooldown_perete > 0) --cooldown_perete;
        miscari_ramase = miscari_ramase_urm;
        miscari_ramase_urm = 1;
        sageti_de_tras = sageti_de_tras_urm;
        sageti_de_tras_urm = 1;
        a_mutat_sageata = false;
        a_schimbat_normala = false;
        la_incheiere_tura();
    }


std::unique_ptr<Sageata> Caracter::Trage(raylib::Vector2 mouse, float forta, const Caracter* inamic) {
        auto s = arc.Trage();
        s->lanseaza(*this, mouse, forta, inamic);
        stats.inregistreaza_trasa();
        return s;
}

void Caracter::IncearcaMiscareHelper(raylib::Vector2 pos_noua) {
    if (miscari_ramase <= 0 || se_misca)
        return;

    pozitieTinta = pos_noua;
    se_misca = true;

    --miscari_ramase;
}

void Caracter::TryMiscare() {
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        IncearcaMiscareHelper(raylib::Vector2{GetHitbox().x + 100.0f, GetHitbox().y});
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        IncearcaMiscareHelper(raylib::Vector2{GetHitbox().x - 100.0f, GetHitbox().y});
}

void Caracter::TrySpawn_perete(const float factor, bool is_p1, const float distanta_zid,
    const float latime_zid, const float inaltime_zid_factor) {
    const float offset_zid = is_p1 ?
    distanta_zid * factor + GetHitbox().width
        : -(distanta_zid * factor + latime_zid * factor);
    if (IsKeyPressed(KEY_P) && cooldown_perete <= 0) {
        const float charH_baza = inaltime_baza * base_scale;
        const float inaltime_perete = (charH_baza + 10.0f) * inaltime_zid_factor;
        const float inaltime_final = inaltime_perete * factor;
        const float spawnX = GetHitbox().x + offset_zid;
        const float spawnY = GetHitbox().y + GetHitbox().height - inaltime_final;
        new Bloc(spawnX, spawnY, latime_zid, inaltime_perete, *this);
        cooldown_perete = 2;
    }
}

void Caracter::TryActiuneArc() {
    if (IsKeyPressed(KEY_Z)) DiscardSageata();
    if (IsKeyPressed(KEY_X)) Change_to_Normala();
    if (IsKeyPressed(KEY_F)) MutaUltimaSageata();
}

void Caracter::IncearcaActiuni(const Caracter* inamic, float& forta_tragere, GameStates& stare,
                               std::vector<std::unique_ptr<Sageata>>& sageti_zbor,
                               float factor, bool is_p1, float distanta_zid, float latime_zid,
                               float inaltime_zid_factor) {
    IncepeTura();
    TryMiscare();
    TrySpawn_perete(factor, is_p1, distanta_zid, latime_zid, inaltime_zid_factor);
    TryActiuneArc();
    ActiuneAditionala();
    IncearcaTragere(inamic, forta_tragere, stare, sageti_zbor);
}


void Caracter::IncearcaTragere(const Caracter* other, float& forta_tragere, GameStates &stare, std::vector<std::unique_ptr<Sageata> > &sageti_zbor) {
    if (!arc.AreSageti()) {
        IncheieTura();
        stare = GameStates::Intermediar;
        rotation = rotatie_baza;
        return;
    }
    const float f = get_factor_scalare();
    const float forta_min = forta_de_baza * f;
    const float forta_max = max_forta_tragere * f;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!trage_arc) {
                trage_arc = true;
                forta_tragere = forta_min;
                rotatie_baza = rotation;
            } else {
                auto s = Trage(GetMousePosition(), forta_tragere, other);
                if (s != nullptr) {
                    sageti_zbor.push_back(std::move(s));
                    --sageti_de_tras;
                }
                trage_arc = false;
                stare = GameStates::Intermediar;
                rotation = rotatie_baza;
            }
        }

        if (trage_arc) {
            float miscareRotita = GetMouseWheelMove();
            if (miscareRotita != 0.0f) {
                forta_tragere += miscareRotita * 50.0f * f;
                if (forta_tragere > forta_max) forta_tragere = forta_max;
                if (forta_tragere < forta_min) forta_tragere = forta_min;
            }
            if (IsKeyPressed(KEY_C)) { trage_arc = false; forta_tragere = 0.0f; }
            raylib::Vector2 pCenter = {GetHitbox().x + GetHitbox().width / 2.0f,
                                       GetHitbox().y + GetHitbox().height / 2.0f};
            raylib::Vector2 mousePos = GetMousePosition();
            float dx = mousePos.x - pCenter.x; float dy = mousePos.y - pCenter.y;
            float dist = std::sqrt(dx*dx + dy*dy);
            rotation = atan2(dy, dx) * 180.0f / PI;
            if (dist > 0) {
                raylib::Vector2 simViteza = {(dx / dist) * forta_tragere, (dy / dist) * forta_tragere};
                raylib::Vector2 punctCurent = pCenter;
                for (int i = 0; i < 30; i++) {
                    simViteza.y += fizica::gravitate * f * 0.03f;
                    raylib::Vector2 punctUrmator = {punctCurent.x + simViteza.x * 0.03f,
                                                    punctCurent.y + simViteza.y * 0.03f};
                    DrawLineEx(punctCurent, punctUrmator, 3.0f * (1.0f - static_cast<float>(i)/80.0f),
                               ColorAlpha(RED, forta_tragere/forta_max + 0.2f));
                    punctCurent = punctUrmator;
                }
            }
        }
}

void Caracter::Update(float dt) {
    UpdateBurn(dt);
    if (se_misca) {
        hitbox.x = Lerp(hitbox.x, pozitieTinta.x, viteza_miscare * dt);
        hitbox.y = Lerp(hitbox.y, pozitieTinta.y, viteza_miscare * dt);

        if (std::abs(hitbox.x - pozitieTinta.x) < 0.1f && std::abs(hitbox.y - pozitieTinta.y) < 0.1f) {
            hitbox.x = pozitieTinta.x;
            hitbox.y = pozitieTinta.y;
            se_misca = false;
        }
    }
}

void Caracter::PushSageata(tipSageti t) {
    arc.PushSageata(t);
}

std::ostream& operator<<(std::ostream& os, const Caracter& c) {
    os << "Hp: " << c.hp << "\nPozitie: (" << c.hitbox.x << ", " << c.hitbox.y << ")\n"
       << "Hitbox: (" << c.hitbox.width << ", " << c.hitbox.height << ")\n" << c.arc;
    return os;
}