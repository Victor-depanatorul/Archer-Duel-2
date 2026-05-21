//
// Created by user on 19.05.2026.
//

#include "fizica.hpp"

namespace fizica {
    std::array<raylib::Vector2, 4> GetCorners(raylib::Rectangle rect, float rotation) {
        std::array<raylib::Vector2, 4> corners;
        raylib::Vector2 origin = { rect.width / 2.0f, rect.height / 2.0f };
        float rad = rotation * (PI / 180.0f);
        float cosA = cosf(rad);
        float sinA = sinf(rad);

        raylib::Vector2 pts[4] = {
            {-origin.x, -origin.y}, {origin.x, -origin.y},
            {origin.x, origin.y}, {-origin.x, origin.y}
        };

        for (int i = 0; i < 4; i++) {
            corners[static_cast<size_t>(i)].x = pts[i].x * cosA - pts[i].y * sinA + rect.x + origin.x;
            corners[static_cast<size_t>(i)].y = pts[i].x * sinA + pts[i].y * cosA + rect.y + origin.y;
        }
        return corners;
    }

    // Funcția SAT Universală
    bool VerColiziune(raylib::Rectangle r1, float rot1, raylib::Rectangle r2, float rot2) {
        auto corners1 = GetCorners(r1, rot1);
        auto corners2 = GetCorners(r2, rot2);

        const std::array<raylib::Vector2, 4>* shapes[2] = { &corners1, &corners2 };

        for (const auto* shape : shapes) {
            const auto& corners = *shape;
            for (size_t i = 0; i < 4; i++) {
                raylib::Vector2 p1 = corners[i];
                raylib::Vector2 p2 = corners[(i + 1) % 4];
                raylib::Vector2 axa = { -(p2.y - p1.y), p2.x - p1.x };

                float lg = sqrtf(axa.x * axa.x + axa.y * axa.y);
                if (lg == 0.0f) continue;
                axa.x /= lg; axa.y /= lg;

                auto getMinMax = [&](const std::array<raylib::Vector2, 4>& c) {
                    float min = INFINITY, max = -INFINITY;
                    for (const auto& p : c) {
                        float dot = p.x * axa.x + p.y * axa.y;
                        if (dot < min) min = dot;
                        if (dot > max) max = dot;
                    }
                    return std::make_pair(min, max);
                };

                auto [min1, max1] = getMinMax(corners1);
                auto [min2, max2] = getMinMax(corners2);

                if (max1 < min2 || max2 < min1) return false;
            }
        }
        return true;
    }
}