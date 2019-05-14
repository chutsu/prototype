#ifndef PROTOTYPE_ESTIMATION_FILTER_HPP
#define PROTOTYPE_ESTIMATION_FILTER_HPP

#include "prototype/core/math.hpp"

namespace proto {

/**
 * A simple complementary filter that uses `gyro` and `accel` measurements to
 * estimate the attitude in `roll` and `pitch`. Where `dt` is the update
 * rate of the `gyro` measurements.
 **/
void complementary_filter(const vec3_t &gyro,
                          const vec3_t &accel,
                          const double dt,
                          double &roll,
                          double &pitch);

} // namespace proto
#endif // PROTOTYPE_ESTIMATION_FILTER_HPP
