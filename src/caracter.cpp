#include "caracter.hpp"
#include "PowerUp.hpp"
#include "exceptii.hpp"

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
Caracter::Caracter(float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(verifica_textura(path)){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
}

Caracter::Caracter(Arc  arc, float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(verifica_textura(path)) ,arc(std::move(arc)){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
}



float Caracter::get_hp() const { return hp; }
std::string Caracter::TipUrmatoareaSageata() const {
    auto* s = arc.VeziUrmatoarea();
    if (s != nullptr)
        return s->nume();
    return "Epuizat";
}

Color Caracter::CuloareUrmatoareaSageate() const {
    auto* s = arc.VeziUrmatoarea();
    if (s != nullptr)
        return s->get_color();
    return BLACK;
}

bool Caracter::InViata() const { return hp > 0; }
bool Caracter::AreSageti() const { return arc.AreSageti(); }

void Caracter::_draw(raylib::Vector2) { // Ignorăm parametrul primit dacă nu e configurat ca origine locală
    auto w = static_cast<float>(textura.GetWidth()) * scale;
    auto h = static_cast<float>(textura.GetHeight()) * scale;
    raylib::Rectangle src = {0, 0, static_cast<float>(textura.GetWidth()), static_cast<float>(textura.GetHeight())};
    raylib::Vector2 origineRotatie = { w / 2.0f, h / 2.0f };
    raylib::Rectangle dest = { hitbox.x + origineRotatie.x, hitbox.y + origineRotatie.y, w, h };
    textura.Draw(src, dest, origineRotatie, rotation);
}

void Caracter::IaDamage(const float dmg) {
    if (dmg >= 0.0f) stats.inregistreaza_damage(dmg);
    hp -= dmg;
}

void Caracter::AplicaOtrava(const int runde) { runde_otrava = std::max(runde_otrava + runde, 5); }

void Caracter::AplicaBurn(const int ture) { runde_burn = std::max(runde_burn, ture); }


void Caracter::UpdateEfect(float dt) {
    if (tura_activa && runde_otrava > 0) {
        IaDamage(dps_otrava);
        --runde_otrava;
    }
    if (tura_activa && runde_burn > 0) {
        IaDamage(burn_dps_frame * dt);
    }
}

void Caracter::OnCollision(Entitate& other) {
        float rad = rotation * (PI / 180.0f);
        float distantaRespingere = other.GetHitbox().width + 10.0f;
        pozitieTinta = raylib::Vector2(hitbox.x, hitbox.y);
        float dx = -std::cos(rad) * distantaRespingere;
        float dy = -std::sin(rad) * distantaRespingere;
        MoveWith(dx, dy);
        se_misca = false;
        IaDamage(1.0f);
}

void Caracter::OnCollision(Sageata &s) {
    s.aplica_efect(*this);
}

void Caracter::IncheieTura() {
        tura_activa = false;
        if (runde_burn > 0) --runde_burn;
        miscari_ramase = miscari_ramase_urm;
        miscari_ramase_urm = 1;
        sageti_de_tras = sageti_de_tras_urm;
        sageti_de_tras_urm = 1;
        a_mutat_sageata = false;
        a_schimbat_normala = false;
    }


std::unique_ptr<Sageata> Caracter::Trage(raylib::Vector2 mouse, float forta, const Caracter* inamic) {
        auto s = arc.Trage();
        s->lanseaza(*this, mouse, forta, inamic);
        stats.inregistreaza_trasa();
        return s;
}

void Caracter::IncearcaMiscare(raylib::Vector2 pos_noua) {
    if (miscari_ramase <= 0 || se_misca)
        return;

    pozitieTinta = pos_noua;
    se_misca = true;

    --miscari_ramase;
}

void Caracter::IncearcaTragere(const Caracter* other, float& forta_tragere, GameStates &stare, std::vector<std::unique_ptr<Sageata> > &sageti_zbor) {
    if (!arc.AreSageti()) {
        IncheieTura();
        stare = GameStates::Intermediar;
        rotation = rotatie_baza;
        return;
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (!trage_arc) {
                trage_arc = true;
                forta_tragere = forta_de_baza;
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
                forta_tragere += miscareRotita * 50.0f;
                if (forta_tragere > max_forta_tragere) forta_tragere = max_forta_tragere;
                if (forta_tragere < forta_de_baza) forta_tragere = forta_de_baza;
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

void Caracter::Update(float dt) {
    UpdateEfect(dt);
    if (se_misca) {
        hitbox.x = Lerp(hitbox.x, pozitieTinta.x, 5.0f * dt);
        hitbox.y = Lerp(hitbox.y, pozitieTinta.y, 5.0f * dt);

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