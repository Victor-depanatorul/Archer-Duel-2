//
// Created by user on 19.05.2026.
//

#ifndef OOP_FIZICA_HPP
#define OOP_FIZICA_HPP
#include <raylib-cpp.hpp>
#include <array>

namespace fizica {
    constexpr float gravitate = 9.8f;
    std::array<raylib::Vector2, 4> GetCorners(raylib::Rectangle rect, float rotation);
    bool VerColiziune(raylib::Rectangle r1, float rot1, raylib::Rectangle r2, float rot2);
}
#endif //OOP_FIZICA_HPP