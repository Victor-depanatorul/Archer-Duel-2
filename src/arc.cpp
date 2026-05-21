//
// Created by user on 13.05.2026.
//

#include "arc.hpp"

// Constructor care creează un set de săgeți de un anumit tip
Arc::Arc(unsigned long long CapacitateArc, tipSageti tip)
    : CapacitateArc(CapacitateArc), Sageti(CapacitateArc, tip) {
}

// Constructor care primește un vector existent de săgeți
Arc::Arc(const std::vector<tipSageti>& Sageti)
    : CapacitateArc(Sageti.size()), Sageti(Sageti) {
}

unsigned long long Arc::get_capacitate() const {
    return CapacitateArc;
}

bool Arc::AreSageti() const {
    return !Sageti.empty();
}

tipSageti Arc::VeziUrmatoarea() const {
    if (!Sageti.empty()) {
        return Sageti.back();
    }
    return tipSageti::Invalid;
}

Sageata Arc::Trage() {
    if (AreSageti()) {
        Sageata s(Sageti.back());
        Sageti.pop_back();
        return s;
    }
    return Sageata(tipSageti::Invalid);
}

// Implementarea operatorului friend
std::ostream& operator<<(std::ostream& os, const Arc& a) {
    os << "Capacitatea arcului: " << a.CapacitateArc << '\n';
    for (const auto& sageata : a.Sageti) {
        os << sageata << '\n';
    }
    return os;
}

