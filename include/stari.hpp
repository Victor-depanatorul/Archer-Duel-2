#ifndef OOP_STARI_HPP
#define OOP_STARI_HPP

#include <memory>
#include "constante.hpp"

class GameDemo;

// Design pattern: State.
// Interfata comuna a tuturor starilor de joc; 'Ruleaza' face logica + randarea
// starii pentru un cadru si poate declansa o tranzitie (prin g.SchimbaStare).
class StareJoc {
public:
    virtual ~StareJoc() = default;
    virtual void Ruleaza(GameDemo& g, float dt) = 0;
};

class StareStartMenu      : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareControale      : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareTuraPlayer     : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareIntermediar    : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StarePauseMenu      : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareGameOver       : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareMeniuGameModes : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareMeniuStatistici: public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };
class StareMeniuPerk      : public StareJoc { public: void Ruleaza(GameDemo& g, float dt) override; };

// Creeaza obiectul de stare corespunzator unei valori din enum-ul GameStates.
std::unique_ptr<StareJoc> creeaza_stare(GameStates tip);

#endif // OOP_STARI_HPP
