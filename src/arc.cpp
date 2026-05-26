#include "arc.hpp"
#include "tipuri_sageti.hpp"
#include <utility>

Arc::Arc(unsigned long long capacitate, tipSageti tip) : capacitate(capacitate) {
    sageti.reserve(capacitate);
    for (unsigned long long i = 0; i < capacitate; ++i)
        sageti.push_back(creeaza_sageata(tip));
}

Arc::Arc(const std::vector<tipSageti>& tipuri) : capacitate(tipuri.size()) {
    sageti.reserve(tipuri.size());
    for (tipSageti t : tipuri)
        sageti.push_back(creeaza_sageata(t));
}

Arc::Arc(const Arc& other) : capacitate(other.capacitate) {
    sageti.reserve(other.sageti.size());
    for (const auto& s : other.sageti)
        sageti.push_back(s->clone());
}

void swap(Arc& a, Arc& b) noexcept {
    using std::swap;
    swap(a.capacitate, b.capacitate);
    swap(a.sageti, b.sageti);
}

Arc& Arc::operator=(Arc other) noexcept {
    swap(*this, other);
    return *this;
}

Arc::~Arc() = default;

bool Arc::AreSageti() const { return !sageti.empty(); }

const Sageata* Arc::VeziUrmatoarea() const {
    return sageti.empty() ? nullptr : sageti.back().get();
}

std::unique_ptr<Sageata> Arc::Trage() {
    if (sageti.empty()) return nullptr;
    auto s = std::move(sageti.back());
    sageti.pop_back();
    return s;
}

void Arc::PushSageata(tipSageti t) {
    if (sageti.size() >= capacitate && !sageti.empty())
        sageti.pop_back();
    sageti.push_back(creeaza_sageata(t));
}

std::ostream& operator<<(std::ostream& os, const Arc& a) {
    os << "Capacitate: " << a.capacitate << '\n';
    for (const auto& s : a.sageti)
        os << "  " << *s << '\n';
    return os;
}

Arc Arc_factory::arc_default() { return Arc(); }

Arc Arc_factory::arc_random(unsigned long long capacitate) {
    std::vector<tipSageti> tipuri(capacitate);
    for (auto& t : tipuri)
        t = static_cast<tipSageti>(MyRand<int>(0, tipSageti::NrTipuri - 1));
    return Arc(tipuri);
}

// Arc Arc_factory::beserker() { return Arc(1); }
//
// Arc Arc_factory::arc_default_divers() {
//     std::vector<tipSageti> tipuri{
//         Normala, Normala, Normala, Normala, Normala, Normala,
//         Giganta, Otravitoare, Aimbot, LifeSteal, Healing
//     };
//     return Arc(tipuri);
// }
