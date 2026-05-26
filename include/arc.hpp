#ifndef OOP_ARC_HPP
#define OOP_ARC_HPP

#include "basic_includes.hpp"
#include "sageata.hpp"
#include <memory>
#include <vector>

class Arc {
    unsigned long long capacitate;
    std::vector<std::unique_ptr<Sageata>> sageti;

public:
    explicit Arc(unsigned long long capacitate = 20, tipSageti tip = Normala);
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

    friend std::ostream& operator<<(std::ostream& os, const Arc& a);
};

class Arc_factory {
public:
    static Arc arc_default();
    static Arc arc_random(unsigned long long capacitate = 20);
    // static Arc beserker();
    // static Arc arc_default_divers();
};

#endif // OOP_ARC_HPP
