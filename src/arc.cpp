#include "arc.hpp"
#include "tipuri_sageti.hpp"
#include "exceptii.hpp"
#include "factory.hpp"
#include <utility>

Arc::Arc(unsigned long long nr_sageti, tipSageti tip) : nr_sageti(nr_sageti) {
    if (nr_sageti > capacitate)
        throw eroare_nr_sageti(nr_sageti);
    sageti.reserve(nr_sageti);
    for (unsigned long long i = 0; i < nr_sageti; ++i)
        sageti.push_back(Sageata_factory::creeaza(tip));
}

Arc::Arc(const std::vector<tipSageti>& tipuri) : nr_sageti(tipuri.size()) {
    if (nr_sageti > capacitate)
        throw eroare_nr_sageti(nr_sageti);
    sageti.reserve(tipuri.size());
    for (tipSageti t : tipuri)
        sageti.push_back(Sageata_factory::creeaza(t));
}

Arc::Arc(const Arc& other) : nr_sageti(other.nr_sageti) {
    sageti.reserve(other.sageti.size());
    for (const auto& s : other.sageti)
        sageti.push_back(s->clone());
}

void swap(Arc& a, Arc& b) noexcept {
    using std::swap;
    swap(a.nr_sageti, b.nr_sageti);
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
    --nr_sageti;
    return s;
}

void Arc::PushSageata(tipSageti t) {
    if (sageti.size() >= capacitate) {
        sageti.pop_back();
        --nr_sageti;
    }
    sageti.push_back(Sageata_factory::creeaza(t));
    ++nr_sageti;
}

std::ostream& operator<<(std::ostream& os, const Arc& a) {
    os << "Sageti: " << a.nr_sageti << "/" << Arc::capacitate << '\n';
    for (const auto& s : a.sageti)
        os << "  " << *s << '\n';
    return os;
}

Arc Arc_factory::creeaza(GameModes mod) {
    static const Factory<GameModes, Arc, NrModuri> fabrica = [] {
        Factory<GameModes, Arc, NrModuri> f;
        f.inregistreaza(Normal,     [] { return std::make_unique<Arc>(); });
        f.inregistreaza(Randomized, [] {
            std::vector<tipSageti> tipuri(20);
            for (auto& t : tipuri)
                t = static_cast<tipSageti>(MyRand<int>(0, tipSageti::NrTipuri - 1));
            return std::make_unique<Arc>(tipuri);
        });
        f.inregistreaza(Beserker,   [] { return std::make_unique<Arc>(1ull); });
        return f;
    }();

    auto a = fabrica.creeaza(mod);
    if (!a) throw eroare_joc("Mod de joc invalid: " + std::to_string(static_cast<int>(mod)));
    return std::move(*a);
}

// Arc Arc_factory::arc_default_divers() {
//     std::vector<tipSageti> tipuri{
//         Normala, Normala, Normala, Normala, Normala, Normala,
//         Giganta, Otravitoare, Aimbot, LifeSteal, Healing
//     };
//     return Arc(tipuri);
// }
