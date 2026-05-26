#include "caracter.hpp"
#include "PowerUp.hpp"

#include <cmath>
#include <utility>

// Constructori
Caracter::Caracter(float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(path){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
}

Caracter::Caracter(Arc  arc, float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(path) ,arc(std::move(arc)){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
}


float Caracter::get_hp() const { return hp; }
std::string Caracter::TipUrmatoareaSageata() const {
    if (auto* s = arc.VeziUrmatoarea())
    return s->nume();
    return "Epuizat";
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

void Caracter::IaDamage(const float dmg) { hp -= dmg; }

void Caracter::AplicaOtrava(const int runde) { runde_otrava = std::max(runde_otrava + runde, 5); }

void Caracter::UpdateEfect() {
    if (runde_otrava > 0) {
        IaDamage(dps_otrava);
        --runde_otrava;
    }
}

void Caracter::OnCollision(Entitate& other) {
        float rad = rotation * (PI / 180.0f);
        float distantaRespingere = other.GetHitbox().width + 10.0f;
        pozitieTinta = raylib::Vector2(hitbox.x, hitbox.y);
        float dx = -std::cos(rad) * distantaRespingere;
        float dy = -std::sin(rad) * distantaRespingere;
        MoveWith(dx, dy);
}

bool Caracter::GetCollision(Sageata &s) {
    if (fizica::VerColiziune(hitbox, rotation, s.get_hitbox(), s.get_rotation())) {
        s.aplica_efect(*this);
        return true;
    }
    return false;
}

std::unique_ptr<Sageata> Caracter::Trage(raylib::Vector2 mouse, float forta, const Caracter* inamic) {
        auto s = arc.Trage();
        if (s) s->lanseaza(*this, mouse, forta, inamic);
        return s;
}

void Caracter::IncearcaMiscare(raylib::Vector2 pos_noua) {
    if (a_mutat || se_misca) return;

    pozitieTinta = pos_noua;
    se_misca = true;

    a_mutat = true;
}

void Caracter::Update(float dt) {
    UpdateEfect();
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