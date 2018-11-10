#ifndef PROTOTYPE_CALIB_CALIB_CAMERA_HPP
#define PROTOTYPE_CALIB_CALIB_CAMERA_HPP

#include <iostream>
#include <string>
#include <memory>

#include <ceres/ceres.h>

#include "prototype/prototype.hpp"
#include "prototype/core.hpp"
#include "prototype/calib/calib.hpp"

namespace prototype {

/**
 * Pose parameter block
 */
struct pose_param_t {
  quat_t q;
  vec3_t t;

  pose_param_t(const mat4_t &T);
  ~pose_param_t();
};

/**
 * Pinhole Radial-tangential calibration residual
 */
struct pinhole_radtan4_residual_t {
  double p_F_[3] = {0.0, 0.0, 0.0}; ///< Object point
  double z_[2] = {0.0, 0.0};        ///< Measurement

  pinhole_radtan4_residual_t(const vec2_t &z, const vec3_t &p_F);
  ~pinhole_radtan4_residual_t();

  /**
   * Calculate residual
   */
  template <typename T>
  bool operator()(const T *const intrinsics,
                  const T *const distortion,
                  const T *const q_CF,
                  const T *const t_CF,
                  T *residual) const;
};

/**
 * Setup camera calibration problem.
 * @returns 0 or -1 for success or failure
 */
int calib_camera_solve(const std::vector<aprilgrid_t> &aprilgrids,
                       pinhole_t &pinhole,
                       radtan4_t &radtan,
                       std::vector<mat4_t> &poses);

/**
 * Perform stats analysis on calibration after performing intrinsics
 * calibration.
 * @returns 0 or -1 for success or failure
 */
template <typename RESIDUAL>
int calib_camera_stats(const std::vector<aprilgrid_t> &aprilgrids,
                       const double *intrinsics,
                       const double *distortion,
                       const std::vector<mat4_t> &poses,
                       const std::string &output_path);

} //  namespace prototype
#include "calib_camera_impl.hpp"
#endif // PROTOTYPE_CALIB_CALIB_CAMERA_HPP