#include "caracter.hpp"
#include <cmath>

// Constructori
Caracter::Caracter(float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(path){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
    sageti_trase.reserve(arc.get_capacitate());
}

Caracter::Caracter(const Arc& arc, float scale, float posX, float posY, float rotation, const char* path, float hp)
    :  Entitate(raylib::Rectangle{posX, posY}, scale, rotation), hp(hp),
    textura(path) ,arc(arc){
    const float w = static_cast<float>(textura.GetWidth())*scale;
    const float h = static_cast<float>(textura.GetHeight())*scale;
    hitbox.SetSize(w, h);
    sageti_trase.reserve(arc.get_capacitate());
}

Caracter::~Caracter() {
    for (auto s : sageti_trase)
        delete s;
}

// Metode Publice
float Caracter::get_hp() const { return hp; }
tipSageti Caracter::TipUrmatoareaSageata() const { return arc.VeziUrmatoarea(); }
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
    const auto s = dynamic_cast<Sageata*>(&other);
    if (s != nullptr) {
        if (s->get_tip() == tipSageti::Otravitoare)
            AplicaOtrava(3);
        IaDamage(s->get_damage());
    }
    else {
        float rad = rotation * (PI / 180.0f);
        float distantaRespingere = 20.0f;
        float dx = -std::cos(rad) * distantaRespingere;
        float dy = -std::sin(rad) * distantaRespingere;

        MoveWith(dx, dy);
    }
}

void Caracter::Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta) {
    if (!arc.AreSageti()) return;

    Sageata* s = new Sageata(arc.Trage());

    raylib::Vector2 centru = { hitbox.x + hitbox.width / 2.0f, hitbox.y + hitbox.height / 2.0f };

    float unghiGrade = 0.0f;

    if (s->get_tip() == tipSageti::Aimbot && tinta != nullptr) {
        raylib::Rectangle tintaRect = tinta->GetHitbox();
        raylib::Vector2 coordTinta = { tintaRect.x + tintaRect.width / 2.0f, tintaRect.y + tintaRect.height / 2.0f };

        float dx = coordTinta.x - centru.x;
        float dy = centru.y - coordTinta.y; // Y-up pentru ecuația balistică
        float v2 = forta * forta;
        float g = fizica::gravitate;
        float radical = (v2 * v2) - g * (g * dx * dx + 2 * dy * v2);

        if (radical >= 0) {
            float unghiBalistic = std::atan2(v2 + std::sqrt(radical), g * dx);
            float directieX = (dx >= 0) ? 1.0f : -1.0f;
            float vx = std::abs(std::cos(unghiBalistic) * forta) * directieX;
            float vy = -std::sin(unghiBalistic) * forta;

            unghiGrade = std::atan2(vy, vx) * (180.0f / PI);
        } else {
            unghiGrade = std::atan2(targetPos.y - centru.y, coordTinta.x - centru.x) * (180.0f / PI);
        }
    } else {
        unghiGrade = std::atan2(targetPos.y - centru.y, targetPos.x - centru.x) * (180.0f / PI);
    }
    s->SetPosition(centru.x, centru.y);
    s->SetRotation(unghiGrade);
    s->UpdateVelocity(forta);

    sageti_trase.push_back(s);
}

void Caracter::UpdateSagetiTrase(float dt, const std::vector<raylib::Rectangle>& others, float max_height) {
    for (int i = static_cast<int>(sageti_trase.size()) - 1; i >= 0; --i) {
        Sageata* s = sageti_trase[i];
        s->Update(dt, *this);

        s->Draw();

        bool distrugeSageata = false;
        for (const auto& re : others) {
            if (fizica::VerColiziune(s->GetHitbox(), s->GetRotation(), re, 0.0f)) {
                distrugeSageata = true;
                TrecereTura = true;
                break;
            }
        }
        if (!distrugeSageata) {
            for (Entitate* e : entitati) {
                if (e==s) continue;
                if (e==this && !s->este_veche()) continue;
                distrugeSageata = e->GetCollision(*s);
                if (distrugeSageata) {
                    const Caracter* c = dynamic_cast<Caracter*>(e);
                    if (c != nullptr) {
                        std::cout << c << std::endl;
                    }
                    break;
                }
            }
        }




        if (s->get_pos().y > max_height) {
            distrugeSageata = true;
            TrecereTura = true;
        }

        if (distrugeSageata) {
            TrecereTura = true;
            delete s;
            sageti_trase.erase(sageti_trase.begin() + i);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Caracter& c) {
    os << "Hp: " << c.hp << "\nPozitie: (" << c.hitbox.x << ", " << c.hitbox.y << ")\n"
       << "Hitbox: (" << c.hitbox.width << ", " << c.hitbox.height << ")\n" << c.arc;
    return os;
}