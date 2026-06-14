#ifndef OOP_ARC_HPP
#define OOP_ARC_HPP

#include "basic_includes.hpp"
#include "sageata.hpp"
#include <memory>
#include <vector>

class Arc {
    static constexpr unsigned long long capacitate = 20;
    unsigned long long nr_sageti;
    std::vector<std::unique_ptr<Sageata>> sageti;

public:
    explicit Arc(unsigned long long nr_sageti = 20, tipSageti tip = Normala);
    explicit Arc(const std::vector<tipSageti>& tipuri);

    Arc(const Arc& other);
    Arc(Arc&& other) noexcept = default;
    Arc& operator=(Arc other) noexcept;
    ~Arc();

    friend void swap(Arc& a, Arc& b) noexcept;

    [[nodiscard]] bool AreSageti() const;
    [[nodiscard]] const Sageata* VeziUrmatoarea() const;

    std::unique_ptr<Sageata> Trage();
    void PushSageata(tipSageti t);

    void PopSageata(){sageti.pop_back();}

    void MutaUltimaSageata() {
        std::swap(sageti.front(), sageti.back());
    }
    friend std::ostream& operator<<(std::ostream& os, const Arc& a);
};

class Arc_factory {
public:
    static Arc creeaza(GameModes mod);
};

#endif // OOP_ARC_HPP
