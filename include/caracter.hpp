#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"
#include "arc.hpp"
#include "sageata.hpp"
#include "statistici.hpp"

class Caracter : public Entitate {
    Arc arc;
    int miscari_ramase = 1;
    int miscari_ramase_urm = 1;
    int sageti_de_tras = 1;
    int sageti_de_tras_urm = 1;
    float armor_multiplier = 1.0f;
    int runde_dmg_multiplier = 0;
    int runde_armor_multiplier = 0;
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
    static constexpr float dps_otrava = 1.5f;
    float vuln_curenta = 1.0f;
    Statistici stats;
    float puncte = 1.0f;

    void IncearcaMiscareHelper(raylib::Vector2 pos_noua);
    std::unique_ptr<Sageata> Trage(raylib::Vector2 targetPos, float forta, const Caracter* tinta = nullptr);
    void UpdateBurn(float dt);
    void IaDamageEfect(float damage);
    void Change_to_Normala() {
        if (a_schimbat_normala) return;
        arc.PopSageata(); arc.PushSageata(tipSageti::Normala);
        a_schimbat_normala = true;
    }
    void MutaUltimaSageata() {
        if (a_mutat_sageata) return;
        arc.MutaUltimaSageata();
        a_mutat_sageata = true;
    }

protected:
    float hp = 100.0f;
    float dmg_multiplier = 1.0f;
    float max_forta_tragere = 1500.0f;
    float viteza_miscare = 5.0f;
    float base_scale = 1.0f;
    std::string path = "assets/textures/pacman3.png";
    raylib::Texture2D textura; //in cazul schimbarii texturii in functie de clasa

    void _draw(raylib::Vector2) override;
    void OnCollision(Entitate& other) override;
    void OnCollision(Sageata& s) override;

    virtual void la_incheiere_tura() {}
    [[nodiscard]] virtual float vulnerabilitate(const Sageata&) const { return 1.0f; }
    [[nodiscard]] virtual float multiplicator_damage_primit() const { return 1.0f; }
    virtual void ActiuneAditionala() {}

    void TryMiscare();
    void TrySpawn_perete(float factor, bool is_p1, float distanta_zid, float latime_zid,
                         float inaltime_zid_factor) const;
    void TryActiuneArc();
    void IncearcaTragere(const Caracter* other, float& forta_tragere, GameStates& stare,
                         std::vector<std::unique_ptr<Sageata>>& sageti_zbor);
public:
    explicit Caracter(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Caracter(Arc arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);

    [[nodiscard]] float inaltime_de_baza() const { return inaltime_baza * base_scale * get_factor_scalare(); }
    [[nodiscard]] virtual std::string nume_clasa() const { return "None"; }
    [[nodiscard]] virtual std::string info_hud() const {
        return "HP: " + std::to_string(static_cast<int>(hp))
             + "\nPuncte: " + std::to_string(get_puncte_afisate())
             + "\nClasa: " + nume_clasa()
             + "\nUrmeaza: " + TipUrmatoareaSageata();
    }
    [[nodiscard]] std::string TipUrmatoareaSageata() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;
    [[nodiscard]] Statistici get_stats() const { return stats; }

    static Castigator determina_castigator(const Caracter& a, const Caracter& b);

    void stats_powerup() { stats.inregistreaza_powerup(); }
    void stats_nimerita() { stats.inregistreaza_nimerita(); }
    void IncepeTura() { tura_activa = true; }
    void IncheieTura();
    void IaDamage(float damage, float multiplier);
    void AplicaOtrava(int runde);
    void AplicaBurn(int runde);
    void PrimesteMultiShot(int n) { sageti_de_tras_urm = n; }
    void PrimesteDoubleMove() { miscari_ramase_urm = 2; }
    void Heal(float cantitate) { hp += cantitate; }

    void SetDmgMultiplier(float mult, bool permanent = false, int runde = 0) {
        dmg_multiplier += mult;
        runde_dmg_multiplier = permanent ? -1 : runde;
    }
    void SetArmorMultiplier(float mult, bool permanent = false, int runde = 0) {
        armor_multiplier = mult;
        runde_armor_multiplier = permanent ? -1 : runde;
    }
    [[nodiscard]] float get_dmg_multiplier() const { return dmg_multiplier; }
    [[nodiscard]] bool mai_are_sageti_de_tras() const { return sageti_de_tras > 0 && AreSageti(); }
    void DiscardSageata() { arc.PopSageata(); }

    void add_puncte(float multiplier) { puncte += stats.acuratete() * multiplier; }
    [[nodiscard]] int get_puncte_afisate() const { return static_cast<int>(puncte * 100); }
    [[nodiscard]] bool poate_plati(int cost) const { return get_puncte_afisate() >= cost; }
    void plateste(int cost) { puncte -= static_cast<float>(cost) / 100.0f; }

    void PushSageata(tipSageti t);
    void Update(float dt) override;

    [[nodiscard]] virtual int durabilitate_zid() const { return 1; }
    virtual void IncearcaActiuni(const Caracter* inamic, float& forta_tragere, GameStates& stare,
                                 std::vector<std::unique_ptr<Sageata>>& sageti_zbor,
                                 float factor, bool is_p1, float distanta_zid, float latime_zid,
                                 float inaltime_zid_factor);

    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif
