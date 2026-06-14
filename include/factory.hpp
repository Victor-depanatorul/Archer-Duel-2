#ifndef OOP_FACTORY_HPP
#define OOP_FACTORY_HPP

#include <array>
#include <functional>
#include <memory>

// Design pattern: Factory (Factory Method generic).
// Factory generic peste chei de tip enum: stocheaza creatorii intr-un array de
// dimensiune N, indexat dupa valoarea cheii. Args... sunt argumentele constructorului.
template <typename Cheie, typename Baza, std::size_t N, typename... Args>
class Factory {
    std::array<std::function<std::unique_ptr<Baza>(Args...)>, N> creatori;

    static std::size_t idx(Cheie k) { return static_cast<std::size_t>(k); }

public:
    void inregistreaza(Cheie k, std::function<std::unique_ptr<Baza>(Args...)> f) {
        creatori.at(idx(k)) = std::move(f);
    }
    [[nodiscard]] std::unique_ptr<Baza> creeaza(Cheie k, Args... args) const {
        const auto& creator = creatori.at(idx(k));
        if (!creator) return nullptr;
        return creator(args...);
    }
};

#endif // OOP_FACTORY_HPP
