#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"
#include "arc.hpp"
#include "sageata.hpp"

class Caracter : public Entitate {
    float hp;
    raylib::Texture2D textura;
    Arc arc;
    bool a_mutat = false;
    bool se_misca = false;
    raylib::Vector2 pozitieTinta = {0.0f, 0.0f};
    int runde_burn = 0;
    bool tura_activa = false;
    static constexpr float burn_dps_frame = 1.0f;

    int runde_otrava = 0;
    static constexpr float dps_otrava = 1.5f;
    void OnCollision(Entitate& other) override;
    void _draw(raylib::Vector2) override;


public:
    // Constructori
    explicit Caracter(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f,
                      const char* PathTextura = "assets/textures/pacman3.png",
                      float hp = 100
                      );

    explicit Caracter(Arc  arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f,
                        float rotation = 0.0f, const char* PathTextura = "assets/textures/pacman3.png",
                        float hp = 100
                      );


    [[nodiscard]] float get_hp() const;
    [[nodiscard]] std::string TipUrmatoareaSageata() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;

    // Logica de joc
    void IncepeTura() {tura_activa = true;}
    void IncheieTura() {tura_activa = false; if (runde_burn > 0) --runde_burn;}
    void IaDamage(float damage);
    void AplicaOtrava(int runde);
    void AplicaBurn(int runde);
    void UpdateEfect(float dt);
    
    std::unique_ptr<Sageata> Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta = nullptr);
    void PushSageata(tipSageti t);
    void IncearcaMiscare(raylib::Vector2 pos_noua);
    bool GetCollision(Sageata& s) override;
    void Update(float dt) override;

    // Operator afișare
    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif