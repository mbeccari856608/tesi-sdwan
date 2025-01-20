#include "SinGenerator.h"
#include <vector>
#include <cmath>
#include <tuple>
#include "PI.h"
#include <iostream>

std::vector<std::tuple<double, double>> generateValues(double shift, int noise) {
    std::vector<std::tuple<double, double>> values;
    double frequency = (2 * M_PI) / 600.0;
    for (double x = 0; x <= 1200; x += 1.0) {
        double y = std::sin(frequency * x) * shift;
        y = std::abs(y);

        if (std::rand() % 3 == 0) {
            int random_offset = (std::rand() % ((noise) + 1));
            // Includiamo anche i negativi
            if (std::rand() % 2 == 0) {
                random_offset *= -1;
            }
            std::cout << random_offset << "\n";
            y += random_offset;
        }

        if (y < 0) y = 0; // Ensure y is not less than 0

        auto a = 41;

        values.emplace_back(x, std::floor(y));
    }
    return values;
}