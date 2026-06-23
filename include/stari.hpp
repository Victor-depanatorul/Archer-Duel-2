#ifndef OOP_STARI_HPP
#define OOP_STARI_HPP

#include <memory>
#include "constante.hpp"

class Game;

// Design pattern: State.
// Interfata comuna a tuturor starilor de joc; 'Ruleaza' face logica + randarea
// starii pentru un cadru si poate declansa o tranzitie (prin g.SchimbaStare).
class StareJoc {
public:
    virtual ~StareJoc() = default;
    virtual void Ruleaza(Game& g, float dt) = 0;
};

class StareStartMenu      : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareControale : public StareJoc {
    int pagina = 0;
public:
    void Ruleaza(Game& g, float dt) override;
};
class StareTuraPlayer     : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareIntermediar    : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StarePauseMenu      : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareGameOver       : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareMeniuGameModes : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareMeniuStatistici: public StareJoc { public: void Ruleaza(Game& g, float dt) override; };
class StareMeniuPerk      : public StareJoc { public: void Ruleaza(Game& g, float dt) override; };

class StareAlegeCaracter : public StareJoc {
    int jucator = 0;
    int previzualizat = -1;
    bool detalii = false;
    tipCaracter ales_p1 = tipCaracter::CaracterNormal;
public:
    void Ruleaza(Game& g, float dt) override;
};

// Creeaza obiectul de stare corespunzator unei valori din enum-ul GameStates.
std::unique_ptr<StareJoc> creeaza_stare(GameStates tip);

#endif // OOP_STARI_HPP
