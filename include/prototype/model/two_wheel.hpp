/**
 * @file
 * @ingroup sim
 */
#ifndef PROTOTYPE_MODEL_TWOWHEEL_HPP
#define PROTOTYPE_MODEL_TWOWHEEL_HPP

#include "prototype/core.hpp"
#include "prototype/control/pid.hpp"

namespace prototype {
/**
 * @addtogroup sim
 * @{
 */

/**
 * Calculate target angular velocity and time taken to traverse a desired
 * circle * trajectory of radius r and velocity v
 *
 * @param r Desired circle radius
 * @param v Desired trajectory velocity
 * @param w Target angular velocity
 * @param time Target time taken to complete circle trajectory
 **/
void circle_trajectory(const double r, const double v, double *w, double *time);

/**
 * Two wheel robot
 */
struct two_wheel_t {
  vec3_t p_G = vec3_t::Zero();
  vec3_t v_G = vec3_t::Zero();
  vec3_t a_G = vec3_t::Zero();
  vec3_t rpy_G = vec3_t::Zero();
  vec3_t w_G = vec3_t::Zero();

  double vx_desired = 0.0;
  double yaw_desired = 0.0;

  pid_t vx_controller{0.1, 0.0, 0.1};
  pid_t yaw_controller{0.1, 0.0, 0.1};

  vec3_t a_B = vec3_t::Zero();
  vec3_t v_B = vec3_t::Zero();
  vec3_t w_B = vec3_t::Zero();

  two_wheel_t() {}

  two_wheel_t(const vec3_t &p_G_,
              const vec3_t &v_G_,
              const vec3_t &rpy_G_) {
    p_G = p_G_;
    v_G = v_G_;
    rpy_G = rpy_G_;
  }

  virtual ~two_wheel_t() {}
};

/**
  * Update
  *
  * @param dt Time difference (s)
  */
void two_wheel_update(two_wheel_t &tm, const double dt);

/** @} group sim */
} //  namespace prototype
#endif // PROTOTYPE_MODEL_TWOWHEEL_HPP