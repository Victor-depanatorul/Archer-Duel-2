#ifndef CARACTER_HPP
#define CARACTER_HPP

#include "basic_includes.hpp"
#include "entitate.hpp"
#include "arc.hpp"
#include "sageata.hpp"
#include "statistici.hpp"
#include <utility>

using LinieHud = std::pair<std::string, Color>;

class Caracter : public Entitate {
    Arc arc;
    int miscari_ramase = 1;
    int miscari_ramase_urm = 1;
    int sageti_de_tras = 1;
    int sageti_de_tras_urm = 1;
    int cooldown_perete = 0;
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
    void Change_to_Normala();
    void MutaUltimaSageata();

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
    [[nodiscard]] virtual float vulnerabilitate(const Sageata&) const;
    [[nodiscard]] virtual float multiplicator_damage_primit() const;
    virtual void ActiuneAditionala() {}

    void TryMiscare();
    void TrySpawn_perete(float factor, bool is_p1, float distanta_zid, float latime_zid,
                         float inaltime_zid_factor);
    void TryActiuneArc();
    void IncearcaTragere(const Caracter* other, float& forta_tragere, GameStates& stare,
                         std::vector<std::unique_ptr<Sageata>>& sageti_zbor);
public:
    explicit Caracter(float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);
    explicit Caracter(Arc arc, float scale = 1.0f, float posX = 0.0f, float posY = 0.0f, float rotation = 0.0f);

    [[nodiscard]] float inaltime_de_baza() const;
    [[nodiscard]] virtual std::string nume_clasa() const;
    [[nodiscard]] virtual std::vector<LinieHud> info_hud() const;
    [[nodiscard]] std::string TipUrmatoareaSageata() const;
    [[nodiscard]] Color CuloareUrmatoareaSageata() const;
    [[nodiscard]] bool InViata() const;
    [[nodiscard]] bool AreSageti() const;
    [[nodiscard]] bool in_miscare() const;
    [[nodiscard]] Statistici get_stats() const;

    static Castigator determina_castigator(const Caracter& a, const Caracter& b);

    void stats_powerup();
    void stats_nimerita();
    void IncepeTura();
    void IncheieTura();
    void IaDamage(float damage, float multiplier);
    void AplicaOtrava(int runde);
    void AplicaBurn(int runde);
    void PrimesteMultiShot(int n);
    void PrimesteDoubleMove();
    void Heal(float cantitate);

    void SetDmgMultiplier(float mult, bool permanent = false, int runde = 0);
    void SetArmorMultiplier(float mult, bool permanent = false, int runde = 0);
    [[nodiscard]] float get_dmg_multiplier() const;
    [[nodiscard]] bool mai_are_sageti_de_tras() const;
    void DiscardSageata();

    void add_puncte(float multiplier);
    [[nodiscard]] int get_puncte_afisate() const;
    [[nodiscard]] bool poate_plati(int cost) const;
    void plateste(int cost);

    void PushSageata(tipSageti t);
    void Update(float dt) override;

    [[nodiscard]] virtual int durabilitate_zid() const;
    virtual void IncearcaActiuni(const Caracter* inamic, float& forta_tragere, GameStates& stare,
                                 std::vector<std::unique_ptr<Sageata>>& sageti_zbor,
                                 float factor, bool is_p1, float distanta_zid, float latime_zid,
                                 float inaltime_zid_factor);

    friend std::ostream& operator<<(std::ostream& os, const Caracter& c);
};

#endif
