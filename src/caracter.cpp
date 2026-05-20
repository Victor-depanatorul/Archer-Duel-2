#include "caracter.hpp"
#include <cmath>

// Inițializarea membrului static
std::vector<Caracter*> Caracter::caractere;

// Metoda privată de înregistrare
void Caracter::InregistreazaCaracter(Caracter* c) {
    auto it = std::ranges::find(caractere.begin(), caractere.end(), c);
    if (it == caractere.end()) {
        caractere.emplace_back(c);
    }
}

// Constructori
Caracter::Caracter(float scale, float posX, float posY, const char* path, float hp)
    : hp(hp), scale(scale), PathTextura(path), textura(path), arc() {
    rect = raylib::Rectangle{ posX, posY,
        static_cast<float>(textura.GetWidth()) * scale,
        static_cast<float>(textura.GetHeight()) * scale };
    sageti_trase.reserve(arc.get_capacitate());
    InregistreazaCaracter(this);
}

Caracter::Caracter(const Arc& arc, float scale, float posX, float posY, const char* path, float hp)
    : hp(hp), scale(scale), PathTextura(path), textura(path), arc(arc) {
    rect = raylib::Rectangle{ posX, posY, static_cast<float>(textura.GetWidth()) * scale, static_cast<float>(textura.GetHeight()) * scale };
    sageti_trase.reserve(arc.get_capacitate());
    InregistreazaCaracter(this);
}

Caracter::Caracter(const Caracter &other)
    : hp(other.hp), scale(other.scale), PathTextura(other.PathTextura),
      textura(other.PathTextura), rect(other.rect), arc(other.arc) {
    InregistreazaCaracter(this);
}

Caracter& Caracter::operator=(const Caracter &other) {
    if (this != &other) {
        hp = other.hp;
        rect = other.rect;
        PathTextura = other.PathTextura;
        textura.Load(other.textura.GetData());
        arc = other.arc;
        scale = other.scale;
    }
    InregistreazaCaracter(this);
    return *this;
}

Caracter::~Caracter() {
    std::erase(caractere, this);
}

// Metode Publice
raylib::Rectangle Caracter::get_rect() const { return rect; }
float Caracter::get_hp() const { return hp; }
tipSageti Caracter::TipUrmatoareaSageata() const { return arc.VeziUrmatoarea(); }
bool Caracter::InViata() const { return hp > 0; }
bool Caracter::AreSageti() const { return arc.AreSageti(); }

void Caracter::DeseneazaCaracter(float rotation) const {
    textura.Draw(rect.GetPosition(), rotation, scale);
}

void Caracter::IaDamage(float damage) { hp -= damage; }

void Caracter::AplicaOtrava(int runde) { runde_otrava = std::max(runde_otrava + runde, 5); }

void Caracter::UpdateEfect() {
    if (runde_otrava > 0) {
        IaDamage(dps_otrava);
        --runde_otrava;
    }
}

void Caracter::set_pozitie(float x, float y) {
    rect.x = x; rect.y = y;
}

bool Caracter::Nimerit(const Sageata& s) {
    raylib::Vector2 varf = s.get_pos();
    if (rect.CheckCollision(varf)) {
        hp -= Sageata::get_damage(s.get_tip());
        if (s.get_tip() == tipSageti::Otravitoare) AplicaOtrava(2);
        return true;
    }
    return false;
}

void Caracter::Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta) {
    if (!arc.AreSageti()) return;

    Sageata s = arc.Trage();
    raylib::Vector2 centru = {rect.x + rect.width / 2, rect.y + rect.height / 2};
    s.MutaSageata(centru.x, centru.y);
    raylib::Vector2 vitezaInitiala{0, 0};

    if (s.get_tip() == tipSageti::Aimbot && tinta != nullptr) {
        raylib::Vector2 coordTinta = {tinta->rect.x + tinta->rect.width/2, tinta->rect.y + tinta->rect.height/2};
        float dx = coordTinta.x - centru.x;
        float dy = centru.y - coordTinta.y;
        float v = forta;
        float g = fizica::gravitate;
        float v2 = v * v;
        float radical = (v2 * v2) - g * (g * dx * dx + 2 * dy * v2);

        if (radical >= 0) {
            float unghi = std::atan2(v2 + std::sqrt(radical), g * dx);
            float directieX = (dx >= 0) ? 1.0f : -1.0f;
            vitezaInitiala.x = std::abs(std::cos(unghi) * v) * directieX;
            vitezaInitiala.y = -std::sin(unghi) * v;
        } else {
            float d = std::sqrt(dx*dx + (centru.y - targetPos.y)*(centru.y - targetPos.y));
            if (d > 0) {
                vitezaInitiala.x = (dx / d) * forta;
                vitezaInitiala.y = ((targetPos.y - centru.y) / d) * forta;
            }
        }
    } else {
        float dx = targetPos.x - centru.x;
        float dy = targetPos.y - centru.y;
        float d = std::sqrt(dx*dx + dy*dy);
        if (d > 0.0f) {
            vitezaInitiala.x = (dx / d) * forta;
            vitezaInitiala.y = (dy / d) * forta;
        }
    }
    s.UpdateVelocity(vitezaInitiala.x, vitezaInitiala.y);
    sageti_trase.emplace_back(s);
}

void Caracter::UpdateSagetiTrase(float dt, const std::vector<raylib::Rectangle>& others, float max_height) {
    for (int i = static_cast<int>(sageti_trase.size()) - 1; i >= 0; --i) {
        Sageata& s = sageti_trase[i];
        s.UpdateVelocity(0.0f, dt * fizica::gravitate);
        s.MiscaSageata(s.get_velocity().x * dt, s.get_velocity().y * dt);

        float w = (s.get_tip() == Giganta) ? 40.0f : 20.0f;
        float h = (s.get_tip() == Giganta) ? 20.0f : 10.0f;

        raylib::Rectangle r(s.get_pos(), raylib::Vector2(w, h));
        r.Draw(Sageata::get_color(s.get_tip()));

        bool distrugeSageata = false;
        for (Caracter* c : caractere) {
            if (c == this && !s.este_veche()) continue;
            if (c->Nimerit(s)) {
                distrugeSageata = true;
                TrecereTura = true;
                std::cout << *c << std::endl;
                break;
            }
        }

        if (!distrugeSageata) {
            for (auto& re : others) {
                if (re.CheckCollision(s.get_pos())) {
                    distrugeSageata = true;
                    TrecereTura = true;
                    break;
                }
            }
        }

        if (s.get_pos().y > max_height) {
            distrugeSageata = true;
            TrecereTura = true;
        }

        if (distrugeSageata) {
            sageti_trase.erase(sageti_trase.begin() + i);
        } else {
            s.CresteVarsta();
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Caracter& c) {
    os << "Hp: " << c.hp << "\nPozitie: (" << c.rect.x << ", " << c.rect.y << ")\n"
       << "Hitbox: (" << c.rect.width << ", " << c.rect.height << ")\n" << c.arc;
    return os;
}