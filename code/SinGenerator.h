#ifndef SIN_GENERATOR_H
#define SIN_GENERATOR_H

#include <vector>
#include <tuple>
#include <cstdint>

std::vector<std::tuple<uint32_t, uint32_t>> generateValues(double shift, int noise);

#endif /* SIN_GENERATOR_H */
