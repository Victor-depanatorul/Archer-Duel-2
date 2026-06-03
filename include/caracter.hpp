#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"
#include "arc.hpp"
#include "sageata.hpp"
#include "statistici.hpp"

class Caracter : public Entitate {
    float hp;
    raylib::Texture2D textura;
    Arc arc;
    int miscari_ramase = 1;
    int miscari_ramase_urm = 1;
    int sageti_de_tras = 1;
    int sageti_de_tras_urm = 1;

    bool se_misca = false;
    bool trage_arc = false;
    bool tura_activa = false;
    bool a_schimbat_normala = false;
    bool a_mutat_sageata = false;

    raylib::Vector2 pozitieTinta = {0.0f, 0.0f};
    int runde_burn = 0;
    int runde_otrava = 0;


    static constexpr float burn_dps_frame = 1.0f;
    static constexpr float forta_de_baza = 400.0f;
    static constexpr float max_forta_tragere = 1500.0f;


    static constexpr float dps_otrava = 1.5f;
    void OnCollision(Entitate& other) override;
    void _draw(raylib::Vector2) override;

    Statistici stats;


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
    [[nodiscard]] Color CuloareUrmatoareaSageate() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;
    [[nodiscard]] Statistici get_stats() const {return stats;}

    void stats_powerup() {stats.inregistreaza_powerup();}
    void stats_nimerita() {stats.inregistreaza_nimerita();}
    // Logica de joc
    void IncepeTura() {
        tura_activa = true;
    }
    void IncheieTura();
    void IaDamage(float damage);
    void AplicaOtrava(int runde);
    void AplicaBurn(int runde);
    void PrimesteMultiShot(int n) {sageti_de_tras_urm = n;}
    void PrimesteDoubleMove() {miscari_ramase_urm = 2;}
    [[nodiscard]] bool mai_are_sageti_de_tras() const {return sageti_de_tras > 0 && AreSageti();}
    void UpdateEfect(float dt);
    void DiscardSageata() {
        arc.PopSageata();
    }
    void Change_to_Normala(){
        if (a_schimbat_normala) return;
        arc.PopSageata(); arc.PushSageata(tipSageti::Normala);
        a_schimbat_normala = true;
    }
    void MutaUltimaSageata() {
        if (a_mutat_sageata) return;
        arc.MutaUltimaSageata();
        a_mutat_sageata = true;
    }
    
    std::unique_ptr<Sageata> Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta = nullptr);
    void PushSageata(tipSageti t);
    void IncearcaMiscare(raylib::Vector2 pos_noua);
    void IncearcaTragere(const Caracter* other, float& forta_tragere, GameStates& stare, std::vector<std::unique_ptr<Sageata>>& sageti_zbor);
    void OnCollision(Sageata& s) override;
    void Update(float dt) override;

    // Operator afișare
    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif