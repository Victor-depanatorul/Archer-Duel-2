//
// Created by user on 13.05.2026.
//

#ifndef OOP_ARC_HPP
#define OOP_ARC_HPP

#include "basic_includes.hpp"
#include "sageata.hpp"

class Arc {
    unsigned long long CapacitateArc;
    std::vector<tipSageti> Sageti;

public:
    // Constructori
    explicit Arc(unsigned long long CapacitateArc = 20, tipSageti tip = Normala);
    explicit Arc(const std::vector<tipSageti>& Sageti);

    // Getters & Verificări
    [[nodiscard]] unsigned long long get_capacitate() const;
    [[nodiscard]] bool AreSageti() const;
    [[nodiscard]] tipSageti VeziUrmatoarea() const;

    // Logica de tragere
    Sageata Trage();

    // Operator de afișare
    friend std::ostream& operator<<(std::ostream& os, const Arc& a);
};

#endif //OOP_ARC_HPP