#ifndef PROTO_VISION_VISION_COMMON_HPP
#define PROTO_VISION_VISION_COMMON_HPP

#include <random>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <opencv2/core/eigen.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include "proto/core/core.hpp"
#include "proto/vision/camera/camera.hpp"

namespace proto {

/**
 * Compare `cv::Mat` whether they are equal
 *
 * @param m1 First matrix
 * @param m2 Second matrix
 * @returns true or false
 */
bool is_equal(const cv::Mat &m1, const cv::Mat &m2);

/**
 * Convert cv::Mat to Eigen::Matrix
 *
 * @param x Input matrix
 * @param y Output matrix
 */
void convert(const cv::Mat &x, matx_t &y);

/**
 * Convert Eigen::Matrix to cv::Mat
 *
 * @param x Input matrix
 * @param y Output matrix
 */
void convert(const matx_t &x, cv::Mat &y);

/**
 * Convert cv::Mat to Eigen::Matrix
 *
 * @param x Input matrix
 * @returns Matrix as Eigen::Matrix
 */
matx_t convert(const cv::Mat &x);

/**
 * Convert Eigen::Matrix to cv::Mat
 *
 * @param x Input matrix
 * @returns Matrix as cv::Mat
 */
cv::Mat convert(const matx_t &x);

/**
 * Convert x to homogenous coordinates
 *
 * @param x Input vector
 * @return Output vector in homogeneous coordinates
 */
vec3_t homogeneous(const vec2_t &x);

/**
 * Convert x to homogenous coordinates
 *
 * @param x Input vector
 * @returns Output vector in homogeneous coordinates
 */
vec4_t homogeneous(const vec3_t &x);

/**
 * Normalize vector of
 */
vec2_t normalize(const vec2_t &x);

/**
 * Convert rvec, tvec into transform matrix
 *
 * @param rvec Rodrigues rotation vector
 * @param tvec Translation vector
 * @returns Transform matrix
 */
mat4_t rvectvec2transform(const cv::Mat &rvec, const cv::Mat &tvec);

/**
 * Skew symmetric matrix
 *
 * @param v Vector
 * @returns Skew symmetric matrix
 */
cv::Matx33d skew(const cv::Vec3d &v);

/**
 * Sort Keypoints
 *
 * @param keypoints
 * @param limit
 * @returns Sorted keypoints by response
 */
std::vector<cv::KeyPoint> sort_keypoints(
    const std::vector<cv::KeyPoint> keypoints, const size_t limit = 0);

/**
 * Convert gray-scale image to rgb image
 *
 * @param image
 *
 * @returns RGB image
 */
cv::Mat gray2rgb(const cv::Mat &image);

/**
 * Convert rgb image to gray-scale image
 *
 * @param image
 *
 * @returns Gray-scale image
 */
cv::Mat rgb2gray(const cv::Mat &image);

/**
 * Create ROI from an image
 *
 * @param[in] image Input image
 * @param[in] width ROI width
 * @param[in] height ROI height
 * @param[in] cx ROI center x-axis
 * @param[in] cy ROI center y-axis
 *
 * @returns ROI
 */
cv::Mat roi(const cv::Mat &image,
            const int width,
            const int height,
            const double cx,
            const double cy);

/**
 * Compare two keypoints based on the response.
 *
 * @param[in] kp1 First keypoint
 * @param[in] kp2 Second keypoint
 * @returns Boolean to denote if first keypoint repose is larger than second
 */
bool keypoint_compare_by_response(const cv::KeyPoint &kp1,
                                  const cv::KeyPoint &kp2);

/**
 * Check to see if rotation matrix is valid (not singular)
 *
 * @param[in] R Rotation matrix
 * @returns Boolean to denote whether rotation matrix is valid
 */
bool is_rot_mat(const cv::Mat &R);

/**
 * Convert rotation matrix to euler angles
 *
 * @param[in] R Rotation matrix
 * @returns Euler angles
 */
cv::Vec3f rot2euler(const cv::Mat &R);

// /**
//  * Outlier rejection using essential matrix
//  *
//  * @param[in] cam0 Camera 0 geometry
//  * @param[in] cam1 Camera 1 geometry
//  * @param[in] T_cam1_cam0 Transform between cam1 and cam0
//  * @param[in] cam0_points Points observed from camera 0
//  * @param[in] cam1_points Points observed from camera 1
//  * @param[in] threshold Threshold
//  * @param[out] inlier_markers
//  */
// template <typename CAMERA_MODEL, typename DISTORTION_MODEL>
// void essential_matrix_outlier_rejection(
//     const camera_geometry_t<CAMERA_MODEL, DISTORTION_MODEL> &cam0,
//     const camera_geometry_t<CAMERA_MODEL, DISTORTION_MODEL> &cam1,
//     const mat4_t &T_cam1_cam0,
//     const std::vector<cv::Point2f> &cam0_points,
//     const std::vector<cv::Point2f> &cam1_points,
//     const double threshold,
//     std::vector<uchar> &inlier_markers);

/**
 * Rescale points
 *
 * @param[in] pts1 Points 1
 * @param[in] pts2 Points 2
 * @returns scaling_factor Scaling factor
 */
float rescale_points(vec2s_t &pts1, std::vector<vec2_t> &pts2);

/**
 * Calculate reprojection error
 *
 * @param[in] measured Measured image pixels
 * @param[in] projected Projected image pixels
 * @returns Reprojection error
 */
double reprojection_error(const vec2s_t &measured, const vec2s_t &projected);

/**
 * Calculate reprojection error
 *
 * @param[in] measured Measured image pixels
 * @param[in] projected Projected image pixels
 * @returns Reprojection error
 */
double reprojection_error(const std::vector<cv::Point2f> &measured,
                          const std::vector<cv::Point2f> &projected);

/**
 * Create feature mask
 *
 * @param[in] image_width Image width
 * @param[in] image_height Image height
 * @param[in] points Points
 * @param[in] patch_width Patch width
 *
 * @returns Feature mask
 */
matx_t feature_mask(const int image_width,
                    const int image_height,
                    const std::vector<cv::Point2f> points,
                    const int patch_width);

/**
 * Create feature mask
 *
 * @param[in] image_width Image width
 * @param[in] image_height Image height
 * @param[in] keypoints Keypoints
 * @param[in] patch_width Patch width
 *
 * @returns Feature mask
 */
matx_t feature_mask(const int image_width,
                    const int image_height,
                    const std::vector<cv::KeyPoint> keypoints,
                    const int patch_width);

/**
 * Create feature mask
 *
 * @param[in] image_width Image width
 * @param[in] image_height Image height
 * @param[in] points Points
 * @param[in] patch_width Patch width
 *
 * @returns Feature mask
 */
cv::Mat feature_mask_opencv(const int image_width,
                            const int image_height,
                            const std::vector<cv::Point2f> points,
                            const int patch_width);

/**
 * Create feature mask
 *
 * @param[in] image_width Image width
 * @param[in] image_height Image height
 * @param[in] keypoints Keypoints
 * @param[in] patch_width Patch width
 *
 * @returns Feature mask
 */
cv::Mat feature_mask_opencv(const int image_width,
                            const int image_height,
                            const std::vector<cv::KeyPoint> keypoints,
                            const int patch_width);

/**
 * Equi undistort image
 *
 * @param[in] K Camera matrix K
 * @param[in] D Distortion vector D
 * @param[in] image Input image
 *
 * @returns Undistorted image using radial-tangential distortion
 */
cv::Mat radtan_undistort_image(const mat3_t &K,
                               const vecx_t &D,
                               const cv::Mat &image);

/**
 * Equi undistort image
 *
 * @param[in] K Camera matrix K
 * @param[in] D Distortion vector D
 * @param[in] image Input image
 * @param[in] balance Balance
 * @param[in,out] Knew New camera matrix K
 *
 * @returns Undistorted image using equidistant distortion
 */
cv::Mat equi_undistort_image(const mat3_t &K,
                             const vecx_t &D,
                             const cv::Mat &image,
                             const double balance,
                             cv::Mat &Knew);
/**
 * Illumination invariant transform.
 *
 * @param[in] image Image
 * @param[in] lambda_1 Lambad 1
 * @param[in] lambda_2 Lambad 2
 * @param[in] lambda_3 Lambad 3
 */
void illum_invar_transform(cv::Mat &image,
                           const double lambda_1,
                           const double lambda_2,
                           const double lambda_3);

double lapm(const cv::Mat &src);

double lapv(const cv::Mat &src);

double teng(const cv::Mat &src, int ksize);

double glvn(const cv::Mat &src);

/******************************************************************************
 * IMPLEMENTATION
 *****************************************************************************/

// template <typename CAMERA_MODEL, typename DISTORTION_MODEL>
// void essential_matrix_outlier_rejection(
//     const camera_geometry_t<CAMERA_MODEL, DISTORTION_MODEL> &cam0,
//     const camera_geometry_t<CAMERA_MODEL, DISTORTION_MODEL> &cam1,
//     const mat4_t &T_cam1_cam0,
//     const std::vector<cv::Point2f> &cam0_points,
//     const std::vector<cv::Point2f> &cam1_points,
//     const double threshold,
//     std::vector<uchar> &inlier_markers) {
//   // Remove outliers using essential matrix
//   // -- Compute the relative rotation between the cam0 frame and cam1 frame
//   const cv::Matx33d R_cam1_cam0 = convert(T_cam1_cam0.block(0, 0, 3, 3));
//   const cv::Vec3d t_cam0_cam1 = convert(T_cam1_cam0.block(0, 3, 3, 1));
//   // -- Compute the essential matrix
//   const cv::Matx33d E = skew(t_cam0_cam1) * R_cam1_cam0;
//   // -- Calculate norm pixel unit
//   const double cam0_fx = cam0.camera_model.fx;
//   const double cam0_fy = cam0.camera_model.fy;
//   const double cam1_fx = cam1.camera_model.fx;
//   const double cam1_fy = cam1.camera_model.fy;
//   const double norm_pixel_unit = 4.0 / (cam0_fx + cam0_fy + cam1_fx +
//   cam1_fy);
//   // -- Further remove outliers based on essential matrix
//   // std::vector<cv::Point2f> cam0_points_ud = undistort(cam0_points);
//   // std::vector<cv::Point2f> cam1_points_ud = undistort(cam1_points);
//   // std::vector<cv::Point2f> cam0_points_ud =
//   // cam0.undistortPoints(cam0_points); std::vector<cv::Point2f>
//   cam1_points_ud
//   // = cam1.undistortPoints(cam1_points); for (size_t i = 0; i <
//   // cam0_points_ud.size(); i++) {
//   //   if (inlier_markers[i] == 0) {
//   //     continue;
//   //   }
//   //
//   //   const cv::Vec3d pt0(cam0_points_ud[i].x, cam0_points_ud[i].y, 1.0);
//   //   const cv::Vec3d pt1(cam1_points_ud[i].x, cam1_points_ud[i].y, 1.0);
//   //   const cv::Vec3d el = E * pt0;
//   //   double err = fabs((pt1.t() * el)[0]) / sqrt(el[0] * el[0] + el[1] *
//   //   el[1]); if (err > threshold * norm_pixel_unit) {
//   //     inlier_markers[i] = 0;
//   //   }
//   // }
// }

} //  namespace proto
#endif // PROTO_VISION_VISION_COMMON_HPP
