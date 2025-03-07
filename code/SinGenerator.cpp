#include "SinGenerator.h"
#include <vector>
#include <cmath>
#include <tuple>
#include "PI.h"
#include <iostream>
#include "Utils.h"

std::vector<std::tuple<uint32_t, uint32_t>> generateValues(double shift, int noise, uint32_t horizontalShift = 0) {
    std::vector<std::tuple<uint32_t, uint32_t>> values;
    double frequency = (2 * M_PI) / 600.0;
    for (uint32_t x = 0; x <= Utils::SimulationDurationInSeconds; x += 1.0) {
        double y = std::sin(frequency * (x + horizontalShift )) * shift;
        y = std::abs(y);

        if (std::rand() % 3 == 0 && noise > 0) {
            int random_offset = (std::rand() % ((noise) + 1));
            if (std::rand() % 2 == 0) {
                random_offset *= -1;
            }
            y += random_offset;
        }

        if (y < 0) y = 0;


        values.emplace_back(x, std::floor(y));
    }
    return values;
}
