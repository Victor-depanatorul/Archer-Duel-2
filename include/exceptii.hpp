#ifndef OOP_EXCEPTII_HPP
#define OOP_EXCEPTII_HPP

#include <stdexcept>
#include <string>

class eroare_joc : public std::runtime_error {
public:
    explicit eroare_joc(const std::string& mesaj) : std::runtime_error(mesaj) {}
};

class eroare_textura : public eroare_joc {
public:
    explicit eroare_textura(const std::string& cale)
        : eroare_joc("Nu s-a putut incarca textura: " + cale) {}
};

class eroare_sageti : public eroare_joc {
public:
    explicit eroare_sageti(int tip)
        : eroare_joc("Tip de sageata invalid: " + std::to_string(tip)) {}
};

class eroare_stare : public eroare_joc {
public:
    explicit eroare_stare(int stare)
        : eroare_joc("Stare de joc invalida: " + std::to_string(stare)) {}
};

class eroare_nr_sageti : public eroare_joc {
public:
    explicit eroare_nr_sageti(unsigned long long nr)
        : eroare_joc("Numar de sageti peste capacitate (max 20): " + std::to_string(nr)) {}
};

#endif // OOP_EXCEPTII_HPP
