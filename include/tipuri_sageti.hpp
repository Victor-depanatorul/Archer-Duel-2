#ifndef OOP_SAGETI_TIPURI_HPP
#define OOP_SAGETI_TIPURI_HPP

#include "sageata.hpp"

class SageataNormala : public Sageata {
public:
    using Sageata::Sageata;
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataOtravitoare : public Sageata {
public:
    using Sageata::Sageata;
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataHealing : public Sageata {
public:
    using Sageata::Sageata;
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataLifeSteal : public Sageata {
public:
    using Sageata::Sageata;
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataGiganta : public Sageata {
public:
    explicit SageataGiganta(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataAimbot : public Sageata {
protected:
    void SetVelocity(raylib::Vector2 tintaMouse, float forta,
                                 const Caracter* inamic) override;
public:
    using Sageata::Sageata;
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

class SageataBurn : public Sageata {
    public:
    explicit SageataBurn(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};
std::unique_ptr<Sageata> creeaza_sageata(tipSageti tip, float x = -1.0f, float y = -1.0f);

class SageataGlassCannon : public Sageata {
    public:
    explicit SageataGlassCannon(float posX = -1.0f, float posY = -1.0f);
    [[nodiscard]] std::unique_ptr<Sageata> clone() const override;
    void aplica_efect(Caracter& tinta) const override;
    [[nodiscard]] Color get_color() const override;
    [[nodiscard]] std::string nume() const override;
};

#endif // OOP_SAGETI_TIPURI_HPP
