#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "arc.hpp"
#include "sageata.hpp"

class Caracter : public Entitate {
    float hp;
    raylib::Texture2D textura;
    Arc arc;
    std::vector<Sageata*> sageti_trase;

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

    explicit Caracter(const Arc& arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f,
                        float rotation = 0.0f, const char* PathTextura = "assets/textures/pacman3.png",
                        float hp = 100
                      );

    ~Caracter() override;

    // Getters
    [[nodiscard]] float get_hp() const;
    [[nodiscard]] tipSageti TipUrmatoareaSageata() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;

    // Logica de joc
    void IaDamage(float damage);
    void AplicaOtrava(int runde);
    void UpdateEfect();
    
    void Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta = nullptr);
    void UpdateSagetiTrase(float dt, const std::vector<raylib::Rectangle>& others, float max_height);

    // Operator afișare
    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif