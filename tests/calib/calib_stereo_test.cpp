#include "prototype/munit.hpp"
#include "prototype/calib/calib.hpp"
#include "prototype/calib/calib_stereo.hpp"

namespace prototype {

#define IMAGE_DIR "/data/euroc_mav/cam_april/mav0/cam0/data"
#define APRILGRID_CONF "test_data/calib/aprilgrid/target.yaml"
#define CAM0_APRILGRID_DATA "/tmp/aprilgrid_test/cam0"
#define CAM1_APRILGRID_DATA "/tmp/aprilgrid_test/cam1"

void test_setup() {
  // Setup calibration target
  calib_target_t target;
  if (calib_target_load(target, APRILGRID_CONF) != 0) {
    FATAL("Failed to load calib target [%s]!", APRILGRID_CONF);
  }

  // Test preprocess data
  const std::string image_dir = IMAGE_DIR;
  const vec2_t image_size{752, 480};
  const double lens_hfov = 98.0;
  const double lens_vfov = 73.0;
  int retval = preprocess_camera_data(target,
                                      image_dir,
                                      image_size,
                                      lens_hfov,
                                      lens_vfov,
                                      CAM0_APRILGRID_DATA);
  if (retval == -1) {
    FATAL("Failed to preprocess camera data!");
  }
}

int test_stereo_residual() {
  // Test load
  std::vector<aprilgrid_t> cam0_aprilgrids;
  int retval = load_camera_calib_data(CAM0_APRILGRID_DATA, cam0_aprilgrids);
  MU_CHECK(retval == 0);
  MU_CHECK(cam0_aprilgrids.size() > 0);
  MU_CHECK(cam0_aprilgrids[0].ids.size() > 0);

  std::vector<aprilgrid_t> cam1_aprilgrids;
  retval = load_camera_calib_data(CAM1_APRILGRID_DATA, cam1_aprilgrids);
  MU_CHECK(retval == 0);
  MU_CHECK(cam1_aprilgrids.size() > 0);
  MU_CHECK(cam1_aprilgrids[0].ids.size() > 0);

  // Setup intrinsic and distortion initialization
  const vec2_t image_size{752, 480};
  const double lens_hfov = 98.0;
  const double lens_vfov = 73.0;
  // -- cam0 intrinsics and distortion
  const double cam0_fx = pinhole_focal_length(image_size(0), lens_hfov);
  const double cam0_fy = pinhole_focal_length(image_size(1), lens_vfov);
  const double cam0_cx = image_size(0) / 2.0;
  const double cam0_cy = image_size(1) / 2.0;
  const vec4_t cam0_intrinsics{cam0_fx, cam0_fy, cam0_cx, cam0_cy};
  const vec4_t cam0_distortion{0.01, 0.0001, 0.0001, 0.0001};
  // -- cam1 intrinsics and distortion
  const double cam1_fx = pinhole_focal_length(image_size(0), lens_hfov);
  const double cam1_fy = pinhole_focal_length(image_size(1), lens_vfov);
  const double cam1_cx = image_size(0) / 2.0;
  const double cam1_cy = image_size(1) / 2.0;
  const vec4_t cam1_intrinsics{cam1_fx, cam1_fy, cam1_cx, cam1_cy};
  const vec4_t cam1_distortion{0.01, 0.0001, 0.0001, 0.0001};

  // Setup cam0 cam1 extrinsics
  // clang-format off
  mat4_t T_C1C0_data;
  T_C1C0_data << 0.999997256477881, 0.002312067192424, 0.000376008102415, -0.110073808127187,
                 -0.002317135723281, 0.999898048506644, 0.014089835846648, 0.000399121547014,
                 -0.000343393120525, -0.014090668452714, 0.999900662637729, -0.000853702503357,
                 0.0, 0.0, 0.0, 1.0;
  // clang-format on

  for (size_t i = 0; i < cam0_aprilgrids.size(); i++) {
    // AprilGrid, keypoint and relative pose observed in cam0
    const auto &grid0 = cam0_aprilgrids[i];
    const auto &cam0_kp = grid0.keypoints[0];
    const auto &T_C0F = grid0.T_CF;

    // AprilGrid, keypoint and relative pose observed in cam1
    const auto &grid1 = cam1_aprilgrids[i];
    const auto &cam1_kp = grid1.keypoints[0];
    const auto &T_C1F = grid1.T_CF;

    // Tag id and corner id
    const auto tag_id = grid0.ids[0];
    const int corner_id = 0;

    // Form pose of fiducial expressed in cam0
    const quat_t q_C0F{T_C0F.block<3, 3>(0, 0)};
    const vec3_t t_C0F{T_C0F.block<3, 1>(0, 3)};

    // Form cam0-cam1 extrinsics
    const quat_t q_C0C1{1.0, 0.0, 0.0, 0.0};
    const vec3_t t_C0C1{0.0, 0.0, 0.0};

    // Form residual and call the functor
    // -- Get the object point
    vec3_t p_F;
    if (aprilgrid_object_point(grid0, tag_id, corner_id, p_F) != 0) {
      LOG_ERROR("Failed to calculate AprilGrid object point!");
    }
    // -- Form residual
    const stereo_residual_t residual{cam0_kp, cam1_kp, p_F};

    // Calculate residual
    double result[4] = {0.0, 0.0, 0.0, 0.0};
    residual(cam0_intrinsics.data(),
             cam0_distortion.data(),
             cam1_intrinsics.data(),
             cam1_distortion.data(),
             q_C0C1.coeffs().data(),
             t_C0C1.data(),
             q_C0F.coeffs().data(),
             t_C0F.data(),
             result);

    std::cout << result[0] << " " << result[1] << " " << result[2] << " " << result[3] << std::endl;

    // Just some arbitrary test to make sure reprojection error is not larger
    // than 100pixels in x or y direction. But often this can be the case ...
    MU_CHECK(result[0] < 100.0);
    MU_CHECK(result[1] < 100.0);
    MU_CHECK(result[2] < 100.0);
    MU_CHECK(result[3] < 100.0);
  }

  return 0;
}

// int test_calib_camera_solve() {
//   // Load calibration data
//   std::vector<aprilgrid_t> aprilgrids;
//   int retval = load_camera_calib_data(APRILGRID_DATA, aprilgrids);
//   MU_CHECK(retval == 0);
//   MU_CHECK(aprilgrids.size() > 0);
//   MU_CHECK(aprilgrids[0].ids.size() > 0);
//
//   // Setup camera intrinsics and distortion
//   const vec2_t image_size{752, 480};
//   const double lens_hfov = 98.0;
//   const double lens_vfov = 73.0;
//   const double fx = pinhole_focal_length(image_size(0), lens_hfov);
//   const double fy = pinhole_focal_length(image_size(1), lens_vfov);
//   const double cx = image_size(0) / 2.0;
//   const double cy = image_size(1) / 2.0;
//   pinhole_t pinhole{fx, fy, cx, cy};
//   radtan4_t radtan{0.01, 0.0001, 0.0001, 0.0001};
//
//   // Test
//   mat4s_t poses;
//   MU_CHECK_EQ(0, calib_camera_solve(aprilgrids, pinhole, radtan, poses));
//   MU_CHECK_EQ(aprilgrids.size(), poses.size());
//
//   // Show results
//   std::cout << "Optimized intrinsics and distortions:" << std::endl;
//   std::cout << pinhole << std::endl;
//   std::cout << radtan << std::endl;
//
//   return 0;
// }
//
// int test_calib_camera_stats() {
//   // Load calibration data
//   std::vector<aprilgrid_t> aprilgrids;
//   int retval = load_camera_calib_data(APRILGRID_DATA, aprilgrids);
//   MU_CHECK(retval == 0);
//   MU_CHECK(aprilgrids.size() > 0);
//   MU_CHECK(aprilgrids[0].ids.size() > 0);
//
//   // Setup camera intrinsics and distortion
//   const vec2_t image_size{752, 480};
//   const double lens_hfov = 98.0;
//   const double lens_vfov = 73.0;
//   const double fx = pinhole_focal_length(image_size(0), lens_hfov);
//   const double fy = pinhole_focal_length(image_size(1), lens_vfov);
//   const double cx = image_size(0) / 2.0;
//   const double cy = image_size(1) / 2.0;
//   pinhole_t pinhole{fx, fy, cx, cy};
//   radtan4_t radtan{0.01, 0.0001, 0.0001, 0.0001};
//
//   // Test
//   mat4s_t poses;
//   MU_CHECK_EQ(0, calib_camera_solve(aprilgrids, pinhole, radtan, poses));
//   MU_CHECK_EQ(aprilgrids.size(), poses.size());
//
//   const double intrinsics[4] = {pinhole.fx, pinhole.fy, pinhole.cx, pinhole.cy};
//   const double distortion[4] = {radtan.k1, radtan.k2, radtan.p1, radtan.p2};
//   calib_camera_stats<stereo_residual_t>(aprilgrids,
//                                                  intrinsics,
//                                                  distortion,
//                                                  poses,
//                                                  "");
//
//   return 0;
// }

void test_suite() {
  test_setup();
  MU_ADD_TEST(test_stereo_residual);
  // MU_ADD_TEST(test_calib_stereo_solve);
  // MU_ADD_TEST(test_calib_stereo_stats);
}

} // namespace prototype

MU_RUN_TESTS(prototype::test_suite);
