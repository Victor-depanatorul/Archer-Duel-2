#ifndef OOP_SAGETI_TIPURI_HPP
#define OOP_SAGETI_TIPURI_HPP

#include "sageata.hpp"
#include <functional>

class SageataDeBaza : public Sageata {
    Color culoare;
    std::string nume_;
    int dmg_perete;
public:
    SageataDeBaza(float posX, float posY, float width, float height, float dmg,
                  Color culoare, std::string nume, int dmg_perete = 1);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override { return culoare; }
    [[nodiscard]] std::string nume() const override { return nume_; }
    [[nodiscard]] bool e_fizica() const override { return true; }
    [[nodiscard]] int damage_perete() const override { return dmg_perete; }
};

class SageataCuEfect : public SageataDeBaza {
public:
    using Efect = std::function<void(Caracter& tinta, Caracter& tragator, float dmg)>;
private:
    Efect efect;
public:
    SageataCuEfect(float posX, float posY, float width, float height, float dmg,
                   Color culoare, std::string nume, Efect efect, int dmg_perete = 1);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] bool e_fizica() const override { return false; }
};

class SageataAimbot : public Sageata {
protected:
    void SetVelocity(raylib::Vector2 tintaMouse, float forta,
                                 const Caracter* inamic) override;
public:
    explicit SageataAimbot(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class Sageata_factory {
public:
    static std::unique_ptr<Sageata> creeaza(tipSageti tip, float x = -1.0f, float y = -1.0f);
};

class SageataGlassCannon : public SageataDeBaza {
    public:
    explicit SageataGlassCannon(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void update(float dt, const std::vector<Entitate*>& obstacole) override;
};

class SageataRandom : public Sageata {
    static constexpr float min_dmg = -50.0f, max_dmg = 50.0f;
    // Intervalul (in secunde) la care se genereaza o noua culoare.
    static constexpr double interval_culoare = 0.06;
    mutable Color culoare_curenta{255, 255, 255, 255};
    mutable double ultima_schimbare = -1.0;
    public:
    explicit SageataRandom(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};
#endif // OOP_SAGETI_TIPURI_HPP
