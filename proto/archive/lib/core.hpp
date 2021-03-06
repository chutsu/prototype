#ifndef PROTO_CORE_HPP
#define PROTO_CORE_HPP

/*****************************************************************************
 * This file is huge, it contains everything from parsing yaml files, linear
 * algebra functions to state estimation code used for robotics.
 *
 * Contents:
 * - Data Type
 * - Macros
 * - Data
 * - Filesystem
 * - Configuration
 * - Algebra
 * - Linear Algebra
 * - Geometry
 * - Differential Geometry
 * - Statistics
 * - Transform
 * - Time
 * - Networking
 * - Interpolation
 * - Control
 * - Measurements
 * - Models
 * - Vision
 * - Parameters
 * - Simulation
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include <dirent.h>

#include <iostream>
#include <fstream>
#include <string>
#include <random>

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Geometry>

namespace proto {

/******************************************************************************
 *                                MACROS
 *****************************************************************************/

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_ERROR(M, ...)                                                      \
  fprintf(stderr,                                                              \
          "\033[31m[ERROR] [%s:%d] " M "\033[0m\n",                            \
          __FILENAME__,                                                        \
          __LINE__,                                                            \
          ##__VA_ARGS__)

#define LOG_INFO(M, ...) fprintf(stdout, "[INFO] " M "\n", ##__VA_ARGS__)
#define LOG_WARN(M, ...)                                                       \
  fprintf(stdout, "\033[33m[WARN] " M "\033[0m\n", ##__VA_ARGS__)

#define FATAL(M, ...)                                                          \
  fprintf(stdout,                                                              \
          "\033[31m[FATAL] [%s:%d] " M "\033[0m\n",                            \
          __FILENAME__,                                                        \
          __LINE__,                                                            \
          ##__VA_ARGS__);                                                      \
  exit(-1)

#ifdef NDEBUG
#define DEBUG(M, ...)
#else
#define DEBUG(M, ...) fprintf(stdout, "[DEBUG] " M "\n", ##__VA_ARGS__)
#endif

#define UNUSED(expr)                                                           \
  do {                                                                         \
    (void) (expr);                                                             \
  } while (0)

#ifndef CHECK
#define CHECK(A, M, ...)                                                       \
  if (!(A)) {                                                                  \
    LOG_ERROR(M, ##__VA_ARGS__);                                               \
    goto error;                                                                \
  }
#endif

/******************************************************************************
 *                                DATA TYPE
 *****************************************************************************/

/* PRECISION TYPE */
// #define PRECISION 1 // Single Precision
#define PRECISION 2 // Double Precision

#if PRECISION == 1
  #define real_t float
#elif PRECISION == 2
  #define real_t double
#else
  #define real_t double
#endif

#define col_major_t Eigen::ColMajor
#define row_major_t Eigen::RowMajor

typedef Eigen::Matrix<real_t, 2, 1> vec2_t;
typedef Eigen::Matrix<real_t, 3, 1> vec3_t;
typedef Eigen::Matrix<real_t, 4, 1> vec4_t;
typedef Eigen::Matrix<real_t, 5, 1> vec5_t;
typedef Eigen::Matrix<real_t, 6, 1> vec6_t;
typedef Eigen::Matrix<real_t, Eigen::Dynamic, 1> vecx_t;
typedef Eigen::Matrix<real_t, 2, 2> mat2_t;
typedef Eigen::Matrix<real_t, 3, 3> mat3_t;
typedef Eigen::Matrix<real_t, 4, 4> mat4_t;
typedef Eigen::Matrix<real_t, Eigen::Dynamic, Eigen::Dynamic> matx_t;
typedef Eigen::Matrix<real_t, 3, 4> mat34_t;
typedef Eigen::Quaternion<real_t> quat_t;
typedef Eigen::AngleAxis<real_t> angle_axis_t;
typedef Eigen::Matrix<real_t, 1, Eigen::Dynamic> row_vector_t;
typedef Eigen::Matrix<real_t, Eigen::Dynamic, 1> col_vector_t;
typedef Eigen::Array<real_t, Eigen::Dynamic, 1> arrayx_t;

typedef Eigen::SparseMatrix<real_t> sp_mat_t;
typedef Eigen::SparseVector<real_t> sp_vec_t;

typedef std::vector<vec2_t, Eigen::aligned_allocator<vec2_t>> vec2s_t;
typedef std::vector<vec3_t, Eigen::aligned_allocator<vec3_t>> vec3s_t;
typedef std::vector<vec4_t, Eigen::aligned_allocator<vec4_t>> vec4s_t;
typedef std::vector<vec5_t, Eigen::aligned_allocator<vec5_t>> vec5s_t;
typedef std::vector<vec6_t, Eigen::aligned_allocator<vec6_t>> vec6s_t;
typedef std::vector<vecx_t> vecxs_t;
typedef std::vector<mat2_t, Eigen::aligned_allocator<mat2_t>> mat2s_t;
typedef std::vector<mat3_t, Eigen::aligned_allocator<mat3_t>> mat3s_t;
typedef std::vector<mat4_t, Eigen::aligned_allocator<mat4_t>> mat4s_t;
typedef std::vector<matx_t, Eigen::aligned_allocator<matx_t>> matxs_t;
typedef std::vector<quat_t, Eigen::aligned_allocator<quat_t>> quats_t;

template <int LENGTH, Eigen::StorageOptions STRIDE_TYPE = Eigen::ColMajor>
using vec_t = Eigen::Matrix<real_t, LENGTH, 1, STRIDE_TYPE>;

template <int ROWS,
          int COLS,
          Eigen::StorageOptions STRIDE_TYPE = Eigen::ColMajor>
using mat_t = Eigen::Matrix<real_t, ROWS, COLS, STRIDE_TYPE>;

template <int ROWS,
          int COLS,
          Eigen::StorageOptions STRIDE_TYPE = Eigen::ColMajor>
using map_mat_t = Eigen::Map<Eigen::Matrix<real_t, ROWS, COLS, STRIDE_TYPE>>;

template <int ROWS>
using map_vec_t = Eigen::Map<Eigen::Matrix<real_t, ROWS, 1>>;

// typedef std::unordered_map<long, std::unordered_map<long, real_t>> mat_hash_t;
// typedef std::vector<std::pair<long int, long int>> mat_indicies_t;

// typedef Eigen::Spline<real_t, 1> Spline1D;
// typedef Eigen::Spline<real_t, 2> Spline2D;
// typedef Eigen::Spline<real_t, 3> Spline3D;

#define SPLINE1D(X, Y, DEG)                                                    \
  Eigen::SplineFitting<Spline1D>::Interpolate(X, DEG, Y)

#define SPLINE2D(X, Y, DEG)                                                    \
  Eigen::SplineFitting<Spline2D>::Interpolate(X, DEG, Y)

#define SPLINE3D(X, Y, DEG)                                                    \
  Eigen::SplineFitting<Spline3D>::Interpolate(X, DEG, Y)


typedef int64_t timestamp_t;
typedef std::vector<timestamp_t> timestamps_t;

/******************************************************************************
 *                                MACROS
 *****************************************************************************/

#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_ERROR(M, ...)                                                      \
  fprintf(stderr,                                                              \
          "\033[31m[ERROR] [%s:%d] " M "\033[0m\n",                            \
          __FILENAME__,                                                        \
          __LINE__,                                                            \
          ##__VA_ARGS__)

#define LOG_INFO(M, ...) fprintf(stdout, "[INFO] " M "\n", ##__VA_ARGS__)
#define LOG_WARN(M, ...)                                                       \
  fprintf(stdout, "\033[33m[WARN] " M "\033[0m\n", ##__VA_ARGS__)

#define FATAL(M, ...)                                                          \
  fprintf(stdout,                                                              \
          "\033[31m[FATAL] [%s:%d] " M "\033[0m\n",                            \
          __FILENAME__,                                                        \
          __LINE__,                                                            \
          ##__VA_ARGS__);                                                      \
  exit(-1)

#ifdef NDEBUG
#define DEBUG(M, ...)
#else
#define DEBUG(M, ...) fprintf(stdout, "[DEBUG] " M "\n", ##__VA_ARGS__)
#endif

#define UNUSED(expr)                                                           \
  do {                                                                         \
    (void) (expr);                                                             \
  } while (0)

#ifndef CHECK
#define CHECK(A, M, ...)                                                       \
  if (!(A)) {                                                                  \
    LOG_ERROR(M, ##__VA_ARGS__);                                               \
    goto error;                                                                \
  }
#endif

/******************************************************************************
 *                                  ALGEBRA
 *****************************************************************************/

/**
 * Sign of number
 *
 * @param[in] x Number to check sign
 * @return
 *    - 0: Number is zero
 *    - 1: Positive number
 *    - -1: Negative number
 */
int sign(const real_t x);

/**
 * Floating point comparator
 *
 * @param[in] f1 First value
 * @param[in] f2 Second value
 * @return
 *    - 0: if equal
 *    - 1: if f1 > f2
 *    - -1: if f1 < f2
 */
int fltcmp(const real_t f1, const real_t f2);

/**
 * Calculate binomial coefficient
 *
 * @param[in] n
 * @param[in] k
 * @returns Binomial coefficient
 */
real_t binomial(const real_t n, const real_t k);

/**
 * Return evenly spaced numbers over a specified interval.
 */
template <typename T>
std::vector<T> linspace(const T start, const T end, const int num) {
  std::vector<T> linspaced;

  if (num == 0) {
    return linspaced;
  }
  if (num == 1) {
    linspaced.push_back(start);
    return linspaced;
  }

  const real_t diff = static_cast<real_t>(end - start);
  const real_t delta = diff / static_cast<real_t>(num - 1);
  for (int i = 0; i < num - 1; ++i) {
    linspaced.push_back(start + delta * i);
  }
  linspaced.push_back(end);
  return linspaced;
}

/******************************************************************************
 *                              LINEAR ALGEBRA
 *****************************************************************************/

/**
 * Print shape of a matrix
 *
 * @param[in] name Name of matrix
 * @param[in] A Matrix
 */
void print_shape(const std::string &name, const matx_t &A);

/**
 * Print shape of a vector
 *
 * @param[in] name Name of vector
 * @param[in] v Vector
 */
void print_shape(const std::string &name, const vecx_t &v);

/**
 * Print array
 *
 * @param[in] name Name of array
 * @param[in] array Target array
 * @param[in] size Size of target array
 */
void print_array(const std::string &name,
                 const real_t *array,
                 const size_t size);

/**
 * Print vector `v` with a `name`.
 */
void print_vector(const std::string &name, const vecx_t &v);

/**
 * Print matrix `m` with a `name`.
 */
void print_matrix(const std::string &name, const matx_t &m);

/**
 * Print quaternion `q` with a `name`.
 */
void print_quaternion(const std::string &name, const quat_t &q);

/**
 * Array to string
 *
 * @param[in] array Target array
 * @param[in] size Size of target array
 * @returns String of array
 */
std::string array2str(const real_t *array, const size_t size);

/**
 * Convert real_t array to Eigen::Vector
 *
 * @param[in] x Input array
 * @param[in] size Size of input array
 * @param[out] y Output vector
 */
void array2vec(const real_t *x, const size_t size, vecx_t &y);

/**
 * Vector to array
 *
 * @param[in] v Vector
 * @returns Array
 */
real_t *vec2array(const vecx_t &v);

/**
 * Matrix to array
 *
 * @param[in] m Matrix
 * @returns Array
 */
real_t *mat2array(const matx_t &m);

/**
 * Quaternion to array
 *
 * *VERY IMPORTANT*: The returned array is (x, y, z, w).
 *
 * @param[in] q Quaternion
 * @returns Array
 */
real_t *quat2array(const quat_t &q);

/**
 * Vector to array
 *
 * @param[in] v Vector
 * @param[out] out Output array
 */
void vec2array(const vecx_t &v, real_t *out);

/**
 * Matrix to array
 *
 * @param[in] m Matrix
 * @param[in] out Output array
 */
void mat2array(const matx_t &m, real_t *out);

/**
 * Matrix to list of vectors
 *
 * @param[in] m Matrix
 * @param[in] row_wise Row wise
 * @returns Vectors
 */
std::vector<vecx_t> mat2vec(const matx_t &m, const bool row_wise = true);

/**
 * Matrix to list of vectors of size 3
 *
 * @param[in] m Matrix
 * @param[in] row_wise Row wise
 * @returns Vectors
 */
vec3s_t mat2vec3(const matx_t &m, const bool row_wise = true);

/**
 * Matrix to list of vectors of size 3
 *
 * @param[in] m Matrix
 * @param[in] row_wise Row wise
 * @returns Vectors
 */
vec2s_t mat2vec2(const matx_t &m, const bool row_wise = true);

/**
 * Vectors to matrix
 */
matx_t vecs2mat(const vec3s_t &vs);

/**
 * Vector to string
 *
 * @param[in] v Vector
 * @param[in] brackets Brakcets around vector string
 * @returns Vector as a string
 */
std::string vec2str(const vecx_t &v, const bool brackets = true);

/**
 * Array to string
 *
 * @param[in] arr Array
 * @param[in] len Length of array
 * @param[in] brackets Brakcets around vector string
 * @returns Array as a string
 */
std::string arr2str(const real_t *arr, const size_t len, bool brackets = true);

/**
 * Matrix to string
 *
 * @param[in] m Matrix
 * @param[in] indent Indent string
 * @returns Array as a string
 */
std::string mat2str(const matx_t &m, const std::string &indent = "  ");

/**
 * Normalize vector x
 */
vec2_t normalize(const vec2_t &x);

/**
 * Normalize vector `v`.
 */
vec3_t normalize(const vec3_t &v);

/**
 * Condition number of `A`.
 */
real_t cond(const matx_t &A);

/**
 * Zeros-matrix
 *
 * @param rows Number of rows
 * @param cols Number of cols
 * @returns Zeros matrix
 */
matx_t zeros(const int rows, const int cols);

/**
 * Zeros square matrix
 *
 * @param size Square size of matrix
 * @returns Zeros matrix
 */
matx_t zeros(const int size);

/**
 * Identity-matrix
 *
 * @param rows Number of rows
 * @param cols Number of cols
 * @returns Identity matrix
 */
matx_t I(const int rows, const int cols);

/**
 * Identity square matrix
 *
 * @param size Square size of matrix
 * @returns Identity square matrix
 */
matx_t I(const int size);

/**
 * Ones-matrix
 *
 * @param rows Number of rows
 * @param cols Number of cols
 * @returns Ones square matrix
 */
matx_t ones(const int rows, const int cols);

/**
 * Ones square matrix
 *
 * @param size Square size of matrix
 * @returns Ones square matrix
 */
matx_t ones(const int size);

/**
 * Horizontally stack matrices A and B
 *
 * @param A Matrix A
 * @param B Matrix B
 * @returns Stacked matrix
 */
matx_t hstack(const matx_t &A, const matx_t &B);

/**
 * Vertically stack matrices A and B
 *
 * @param A Matrix A
 * @param B Matrix B
 * @returns Stacked matrix
 */
matx_t vstack(const matx_t &A, const matx_t &B);

/**
 * Diagonally stack matrices A and B
 *
 * @param A Matrix A
 * @param B Matrix B
 * @returns Stacked matrix
 */
matx_t dstack(const matx_t &A, const matx_t &B);

/**
 * Skew symmetric-matrix
 *
 * @param w Input vector
 * @returns Skew symmetric matrix
 */
mat3_t skew(const vec3_t &w);

/**
 * Skew symmetric-matrix squared
 *
 * @param w Input vector
 * @returns Skew symmetric matrix squared
 */
mat3_t skewsq(const vec3_t &w);

/**
 * Enforce Positive Semi-Definite
 *
 * @param A Input matrix
 * @returns Positive semi-definite matrix
 */
matx_t enforce_psd(const matx_t &A);

/**
 * Null-space of A
 *
 * @param A Input matrix
 * @returns Null space of A
 */
matx_t nullspace(const matx_t &A);

/**
 * Check if two matrices `A` and `B` are equal.
 */
bool equals(const matx_t &A, const matx_t &B);

/**
 * Load std::vector of real_ts to an Eigen::Matrix
 *
 * @param[in] x Matrix values
 * @param[in] rows Number of matrix rows
 * @param[in] cols Number of matrix colums
 * @param[out] y Output matrix
 */
void load_matrix(const std::vector<real_t> &x,
                 const int rows,
                 const int cols,
                 matx_t &y);

/**
 * Load an Eigen::Matrix into a std::vector of real_ts
 *
 * @param[in] A Matrix
 * @param[out] x Output vector of matrix values
 */
void load_matrix(const matx_t A, std::vector<real_t> &x);

/** Pseudo Inverse via SVD **/
matx_t pinv(const matx_t &A, const real_t tol=1e-4);

/** Rank of matrix `A` **/
long int rank(const matx_t &A);

/** Check whether matrix `A` full Rank **/
bool full_rank(const matx_t &A);

// /**
//  * Perform Schur's Complement
//  */
// int schurs_complement(matx_t &H, vecx_t &b,
//                       const size_t m, const size_t r,
//                       const bool precond=false, const bool debug=false);


// /**
//  * Recover covariance(i, l) (a specific value in the covariance matrix) from
//  * the upper triangular matrix `U` with precomputed diagonal vector containing
//  * `diag(U)^{-1}`. Computed covariances will be stored in the `hash` to avoid
//  * recomputing the value again.
//  */
// real_t covar_recover(const long i, const long l,
//                      const matx_t &U, const vecx_t &diag,
//                      mat_hash_t &hash);
//
// /**
//  * From the Hessian matrix `H`, recover the covariance values defined in
//  * `indicies`. Returns a matrix hashmap of covariance values.
//  */
// mat_hash_t covar_recover(const matx_t &H, const mat_indicies_t &indicies);

/******************************************************************************
 *                                 Geometry
 *****************************************************************************/

/**
 * Sinc function.
 */
real_t sinc(const real_t x);

/**
 * Degrees to radians
 *
 * @param[in] d Degree to be converted
 * @return Degree in radians
 */
real_t deg2rad(const real_t d);

/**
 * Degrees to radians
 *
 * @param[in] d Degree to be converted
 * @return Degree in radians
 */
vec3_t deg2rad(const vec3_t d);

/**
 * Radians to degree
 *
 * @param[in] r Radian to be converted
 * @return Radian in degrees
 */
real_t rad2deg(const real_t r);

/**
 * Radians to degree
 *
 * @param[in] r Radian to be converted
 * @return Radian in degrees
 */
vec3_t rad2deg(const vec3_t &r);

/**
 * Wrap angle in degrees to 180
 *
 * @param[in] d Degrees
 * @return Angle wraped to 180
 */
real_t wrap180(const real_t d);

/**
 * Wrap angle in degrees to 360
 *
 * @param[in] d Degrees
 * @return Angle wraped to 360
 */
real_t wrap360(const real_t d);

/**
 * Wrap angle in radians to PI
 *
 * @param[in] r Radians
 * @return Angle wraped to PI
 */
real_t wrapPi(const real_t r);

/**
 * Wrap angle in radians to 2 PI
 *
 * @param[in] r Radians
 * @return Angle wraped to 2 PI
 */
real_t wrap2Pi(const real_t r);

/**
 * Create a circle point of radius `r` at angle `theta` radians.
 */
vec2_t circle(const real_t r, const real_t theta);

/**
 * Create the sphere point with sphere radius `rho` at longitude `theta`
 * [radians] and latitude `phi` [radians].
 */
vec3_t sphere(const real_t rho, const real_t theta, const real_t phi);

/**
 * Create look at matrix.
 */
mat4_t lookat(const vec3_t &cam_pos,
              const vec3_t &target,
              const vec3_t &up_axis = vec3_t{0.0, -1.0, 0.0});

/**
 * Cross-Track error based on waypoint line between p1, p2, and robot position
 *
 * @param[in] p1 Waypoint 1
 * @param[in] p2 Waypoint 2
 * @param[in] pos Robot position
 * @return Cross track error
 */
real_t cross_track_error(const vec2_t &p1, const vec2_t &p2, const vec2_t &pos);

/**
 * Check if point `pos` is left or right of line formed by `p1` and `p2`
 *
 * @param[in] p1 Waypoint 1
 * @param[in] p2 Waypoint 2
 * @param[in] pos Robot position
 * @returns
 *    - 1: Point is left of waypoint line formed by `p1` and `p2`
 *    - 2: Point is right of waypoint line formed by `p1` and `p2`
 *    - 0: Point is colinear with waypoint line formed by `p1` and `p2`
 */
int point_left_right(const vec2_t &p1, const vec2_t &p2, const vec2_t &pos);

/**
 * Calculate closest point given waypoint line between `p1`, `p2` and robot
 * position
 *
 * @param[in] p1 Waypoint 1
 * @param[in] p2 Waypoint 2
 * @param[in] p3 Robot position
 * @param[out] closest Closest point
 * @returns
 *    Unit number denoting where the closest point is on waypoint line. For
 *    example, a return value of 0.5 denotes the closest point is half-way
 *    (50%) of the waypoint line, alternatively a negative number denotes the
 *    closest point is behind the first waypoint.
 */
real_t closest_point(const vec2_t &p1,
                     const vec2_t &p2,
                     const vec2_t &p3,
                     vec2_t &closest);

#define EARTH_RADIUS_M 6378137.0

/**
 * Calculate new latitude and logitude coordinates with an offset in North and
 * East direction.
 *
 * IMPORTANT NOTE: This function is only an approximation. As such do not rely
 * on this function for precise latitude, longitude offsets.
 *
 * @param lat_ref Latitude of origin (decimal format)
 * @param lon_ref Longitude of origin (decimal format)
 * @param offset_N Offset in North direction (meters)
 * @param offset_E Offset in East direction (meters)
 * @param lat_new New latitude (decimal format)
 * @param lon_new New longitude (decimal format)
 */
void latlon_offset(real_t lat_ref,
                   real_t lon_ref,
                   real_t offset_N,
                   real_t offset_E,
                   real_t *lat_new,
                   real_t *lon_new);

/**
 * Calculate difference in distance in North and East from two GPS coordinates
 *
 * IMPORTANT NOTE: This function is only an approximation. As such do not rely
 * on this function for precise latitude, longitude diffs.
 *
 * @param lat_ref Latitude of origin (decimal format)
 * @param lon_ref Longitude of origin (decimal format)
 * @param lat Latitude of point of interest (decimal format)
 * @param lon Longitude of point of interest (decimal format)
 * @param dist_N Distance of point of interest in North axis [m]
 * @param dist_E Distance of point of interest in East axis [m]
 */
void latlon_diff(real_t lat_ref,
                 real_t lon_ref,
                 real_t lat,
                 real_t lon,
                 real_t *dist_N,
                 real_t *dist_E);

/**
 * Calculate Euclidean distance between two GPS coordintes
 *
 * IMPORTANT NOTE: This function is only an approximation. As such do not rely
 * on this function for precise latitude, longitude distance.
 *
 * @param lat_ref Latitude of origin (decimal format)
 * @param lon_ref Longitude of origin (decimal format)
 * @param lat Latitude of point of interest (decimal format)
 * @param lon Longitude of point of interest (decimal format)
 *
 * @returns Euclidean distance between two GPS coordinates [m]
 */
real_t latlon_dist(real_t lat_ref, real_t lon_ref, real_t lat, real_t lon);

/*****************************************************************************
 *                         DIFFERENTIAL GEOMETRY
 *****************************************************************************/

namespace lie {

mat3_t Exp(const vec3_t &phi);
vec3_t Log(const mat3_t &C);
mat3_t Jr(const vec3_t &psi);

} // namespace lie


/******************************************************************************
 *                                STATISTICS
 *****************************************************************************/

/**
 * Create random integer
 *
 * @param[in] ub Upper bound
 * @param[in] lb Lower bound
 * @return Random integer
 */
int randi(const int ub, const int lb);

/**
 * Create random real_t
 *
 * @param[in] ub Upper bound
 * @param[in] lb Lower bound
 * @return Random floating point
 */
real_t randf(const real_t ub, const real_t lb);

/**
 * Sum values in vector.
 *
 * @param[in] x Array of numbers
 * @return Sum of vector
 */
real_t sum(const std::vector<real_t> &x);

/**
 * Calculate median given an array of numbers
 *
 * @param[in] v Array of numbers
 * @return Median of given array
 */
real_t median(const std::vector<real_t> &v);

/**
 * Mean vector
 *
 * @param[in] x List of vectors
 * @return Mean vector
 */
vec3_t mean(const vec3s_t &x);

/**
 * Root Mean Squared Error.
 */
real_t rmse(const std::vector<real_t> &residuals);

/**
 * Shannon Entropy of a given covariance matrix `covar`.
 */
real_t shannon_entropy(const matx_t &covar);

/**
 * Multivariate normal.
 */
vec3_t mvn(std::default_random_engine &engine,
           const vec3_t &mu = vec3_t{0.0, 0.0, 0.0},
           const vec3_t &stdev = vec3_t{1.0, 1.0, 1.0});

/**
 * Gassian normal.
 * http://c-faq.com/lib/gaussian.html
 */
real_t gauss_normal();

/*****************************************************************************
 *                               TRANSFORM
 *****************************************************************************/

/**
 * Form a 4x4 homogeneous transformation matrix from a pointer to real_t array
 * containing (quaternion + translation) 7 elements: (qw, qx, qy, qz, x, y, z)
 */
mat4_t tf(const double *params);

/**
 * Form a 4x4 homogeneous transformation matrix from a pointer to real_t array
 * containing (quaternion + translation) 7 elements: (qw, qx, qy, qz, x, y, z)
 */
mat4_t tf(const vecx_t &params);

/**
 * Form a 4x4 homogeneous transformation matrix from a
 * rotation matrix `C` and translation vector `r`.
 */
mat4_t tf(const mat3_t &C, const vec3_t &r);

/**
 * Form a 4x4 homogeneous transformation matrix from a
 * Hamiltonian quaternion `q` and translation vector `r`.
 */
mat4_t tf(const quat_t &q, const vec3_t &r);

/**
 * Extract rotation from transform
 */
inline mat3_t tf_rot(const mat4_t &tf) { return tf.block<3, 3>(0, 0); }

/**
 * Extract rotation and convert to quaternion from transform
 */
inline quat_t tf_quat(const mat4_t &tf) { return quat_t{tf.block<3, 3>(0, 0)}; }

/**
 * Extract translation from transform
 */
inline vec3_t tf_trans(const mat4_t &tf) { return tf.block<3, 1>(0, 3); }


/**
 * Perturb the rotation element in the tranform `T` by `step_size` at index
 * `i`. Where i = 0 for x-axis, i = 1 for y-axis, and i = 2 for z-axis.
 */
mat4_t tf_perturb_rot(const mat4_t &T, real_t step_size, const int i);

/**
 * Perturb the translation element in the tranform `T` by `step_size` at index
 * `i`. Where i = 0 for x-axis, i = 1 for y-axis, and i = 2 for z-axis.
 */
mat4_t tf_perturb_trans(const mat4_t &T, real_t step_size, const int i);

/**
 * Transform point `p` with transform `T`.
 */
vec3_t tf_point(const mat4_t &T, const vec3_t &p);

/**
 * Rotation matrix around x-axis (counter-clockwise, right-handed).
 * @returns Rotation matrix
 */
mat3_t rotx(const real_t theta);

/**
 * Rotation matrix around y-axis (counter-clockwise, right-handed).
 * @returns Rotation matrix
 */
mat3_t roty(const real_t theta);

/**
 * Rotation matrix around z-axis (counter-clockwise, right-handed).
 * @returns Rotation matrix
 */
mat3_t rotz(const real_t theta);

/**
 * Convert euler sequence 123 to rotation matrix R
 * This function assumes we are performing a body fixed intrinsic rotation.
 *
 * Source:
 *
 *     Kuipers, Jack B. Quaternions and Rotation Sequences: A Primer with
 *     Applications to Orbits, Aerospace, and Virtual Reality. Princeton, N.J:
 *     Princeton University Press, 1999. Print.
 *
 *     Page 86.
 *
 * @returns Rotation matrix
 */
mat3_t euler123(const vec3_t &euler);

/**
 * Convert euler sequence 321 to rotation matrix R
 * This function assumes we are performing a body fixed intrinsic rotation.
 *
 * Source:
 *
 *     Kuipers, Jack B. Quaternions and Rotation Sequences: A Primer with
 *     Applications to Orbits, Aerospace, and Virtual Reality. Princeton, N.J:
 *     Princeton University Press, 1999. Print.
 *
 *     Page 86.
 *
 * @returns Rotation matrix
 */
mat3_t euler321(const vec3_t &euler);

/**
 * Convert roll, pitch and yaw to quaternion.
 */
quat_t euler2quat(const vec3_t &euler);

/**
 * Convert rotation vectors to rotation matrix using measured acceleration
 * `a_m` from an IMU and gravity vector `g`.
 */
mat3_t vecs2rot(const vec3_t &a_m, const vec3_t &g);

/**
 * Convert rotation vector `rvec` to rotation matrix.
 */
mat3_t rvec2rot(const vec3_t &rvec, const real_t eps = 1e-5);

/**
 * Convert quaternion to euler angles.
 */
vec3_t quat2euler(const quat_t &q);

/**
 * Convert quaternion to rotation matrix.
 */
mat3_t quat2rot(const quat_t &q);

/**
 * Convert small angle euler angle to quaternion.
 */
quat_t quat_delta(const vec3_t &dalpha);

/**
 * Return left quaternion product matrix.
 */
mat4_t quat_lmul(const quat_t &q);

/**
 * Return left quaternion product matrix (but only for x, y, z components).
 */
mat3_t quat_lmul_xyz(const quat_t &q);

/**
 * Return right quaternion product matrix.
 */
mat4_t quat_rmul(const quat_t &q);

/**
 * Return right quaternion product matrix (but only for x, y, z components).
 */
mat3_t quat_rmul_xyz(const quat_t &q);

/**
 * Return only the x, y, z, components of a quaternion matrix.
 */
mat3_t quat_mat_xyz(const mat4_t &Q);

/**
 * Add noise to rotation matrix `rot`, where noise `n` is in degrees.
 */
mat3_t add_noise(const mat3_t &rot, const real_t n);

/**
 * Add noise to position vector `pos`, where noise `n` is in meters.
 */
vec3_t add_noise(const vec3_t &pos, const real_t n);

/**
 * Add noise to transform `pose`, where `pos_n` is in meters and `rot_n` is in
 * degrees.
 */
mat4_t add_noise(const mat4_t &pose, const real_t pos_n, const real_t rot_n);

/**
 * Initialize attitude using IMU gyroscope `w_m` and accelerometer `a_m`
 * measurements. The calculated attitude outputted into to `C_WS`. Note: this
 * function does not calculate initial yaw angle in the world frame. Only the
 * roll, and pitch are inferred from IMU measurements.
 */
void imu_init_attitude(const vec3s_t w_m,
                       const vec3s_t a_m,
                       mat3_t &C_WS,
                       const size_t buffer_size = 50);

/*****************************************************************************
 *                                    TIME
 *****************************************************************************/

/**
 * Print timestamp.
 */
void timestamp_print(const timestamp_t &ts, const std::string &prefix = "");

/**
 * Convert seconds to timestamp.
 */
timestamp_t ts2sec(const real_t seconds);

/**
 * Convert ts to second.
 */
real_t ts2sec(const timestamp_t &ts);

/**
 * Convert nano-second to second.
 */
real_t ns2sec(const uint64_t ns);

/**
 * Start timer.
 */
struct timespec tic();

/**
 * Stop timer and return number of seconds.
 */
float toc(struct timespec *tic);

/**
 * Stop timer and return miliseconds elasped.
 */
float mtoc(struct timespec *tic);

/**
 * Get time now in milliseconds since epoch
 */
real_t time_now();

/**
 * Profiler
 */
struct profiler_t {
  std::map<std::string, timespec> timers;
  std::map<std::string, float> record;

  profiler_t() {}

  void start(const std::string &key) {
    timers[key] = tic();
  }

  float stop(const std::string &key, const bool verbose=false) {
    record[key] = toc(&timers[key]);
    if (verbose) {
      printf("[%s]: %.4fs\n", key.c_str(), stop(key));
    }

    return record[key];
  }
};

// // /******************************************************************************
// //  *                              INTERPOLATION
// //  *****************************************************************************/
// //
// // /**
// //  * Linear interpolation between two points.
// //  *
// //  * @param[in] a First point
// //  * @param[in] b Second point
// //  * @param[in] t Unit number
// //  * @returns Linear interpolation
// //  */
// // template <typename T>
// // T lerp(const T &a, const T &b, const real_t t) {
// //   return a * (1.0 - t) + b * t;
// // }
// //
// // /**
// //  * Slerp
// //  */
// // quat_t slerp(const quat_t &q_start, const quat_t &q_end, const real_t alpha);
// //
// // /**
// //  * Interpolate between two poses `p0` and `p1` with parameter `alpha`.
// //  */
// // mat4_t interp_pose(const mat4_t &p0, const mat4_t &p1, const real_t alpha);
// //
// // /**
// //  * Interpolate `poses` where each pose has a timestamp in `timestamps` and the
// //  * interpolation points in time are in `interp_ts`. The results are recorded
// //  * in `interp_poses`.
// //  * @returns 0 for success, -1 for failure
// //  */
// // void interp_poses(const timestamps_t &timestamps,
// //                   const mat4s_t &poses,
// //                   const timestamps_t &interp_ts,
// //                   mat4s_t &interped_poses,
// //                   const real_t threshold = 0.001);
// //
// // /**
// //  * Get the closest pose in `poses` where each pose has a timestamp in
// //  * `timestamps` and the target points in time are in `target_ts`. The results
// //  * are recorded in `result`.
// //  * @returns 0 for success, -1 for failure
// //  */
// // void closest_poses(const timestamps_t &timestamps,
// //                    const mat4s_t &poses,
// //                    const timestamps_t &interp_ts,
// //                    mat4s_t &result);
// //
// //
// // /**
// //  * Let `t0` and `t1` be timestamps from two signals. If one of them is measured
// //  * at a higher rate, the goal is to interpolate the lower rate signal so that
// //  * it aligns with the higher rate one.
// //  *
// //  * This function will determine which timestamp deque will become the reference
// //  * signal and the other will become the target signal. Based on this the
// //  * interpolation points will be based on the reference signal.
// //  *
// //  * Additionally, this function ensures the interpolation timestamps are
// //  * achievable by:
// //  *
// //  * - interp start > target start
// //  * - interp end < target end
// //  *
// //  * **Note**: This function will not include timestamps from the target
// //  * (lower-rate) signal. The returned interpolation timestamps only returns
// //  * **interpolation points** to match the reference signal (higher-rate).
// //  *
// //  * @returns Interpolation timestamps from two timestamp deques `t0` and `t1`.
// //  */
// // std::deque<timestamp_t> lerp_timestamps(const std::deque<timestamp_t> &t0,
// //                                         const std::deque<timestamp_t> &t1);
// //
// // /**
// //  * Given the interpolation timestamps `lerp_ts`, target timestamps
// //  * `target_ts` and target data `target_data`. This function will:
// //  *
// //  * 1: Interpolate the `target_data` at the interpolation points defined by
// //  *    `target_ts`.
// //  * 2: Disgard data that are not in the target timestamp
// //  */
// // void lerp_data(const std::deque<timestamp_t> &lerp_ts,
// //                std::deque<timestamp_t> &target_ts,
// //                std::deque<vec3_t> &target_data,
// //                const bool keep_old = false);
// //
// // /**
// //  * Given two data signals with timestamps `ts0`, `vs0`, `ts1`, and `vs1`, this
// //  * function determines which data signal is at a lower rate and performs linear
// //  * interpolation inorder to synchronize against the higher rate data signal.
// //  *
// //  * The outcome of this function is that both data signals will have:
// //  *
// //  * - Same number of timestamps.
// //  * - Lower-rate data will be interpolated against the higher rate data.
// //  *
// //  * **Note**: This function will drop values from the start and end of both
// //  * signals inorder to synchronize them.
// //  */
// // void lerp_data(std::deque<timestamp_t> &ts0,
// //                std::deque<vec3_t> &vs0,
// //                std::deque<timestamp_t> &ts1,
// //                std::deque<vec3_t> &vs1);

// // /**
// //  * Continuous trajectory generator
// //  */
// // struct ctraj_t {
// //   const timestamps_t timestamps;
// //   const vec3s_t positions;
// //   const quats_t orientations;
// //
// //   const real_t ts_s_start;
// //   const real_t ts_s_end;
// //   const real_t ts_s_gap;
// //
// //   Spline3D pos_spline;
// //   Spline3D rvec_spline;
// //
// //   ctraj_t(const timestamps_t &timestamps,
// //           const vec3s_t &positions,
// //           const quats_t &orientations);
// // };
// //
// // /**
// //  * Container for multiple continuous trajectories
// //  */
// // typedef std::vector<ctraj_t> ctrajs_t;
// //
// // /**
// //  * Initialize continuous trajectory.
// //  */
// // void ctraj_init(ctraj_t &ctraj);
// //
// // /**
// //  * Calculate pose `T_WB` at timestamp `ts`.
// //  */
// // mat4_t ctraj_get_pose(const ctraj_t &ctraj, const timestamp_t ts);
// //
// // /**
// //  * Calculate velocity `v_WB` at timestamp `ts`.
// //  */
// // vec3_t ctraj_get_velocity(const ctraj_t &ctraj, const timestamp_t ts);
// //
// // /**
// //  * Calculate acceleration `a_WB` at timestamp `ts`.
// //  */
// // vec3_t ctraj_get_acceleration(const ctraj_t &ctraj, const timestamp_t ts);
// //
// // /**
// //  * Calculate angular velocity `w_WB` at timestamp `ts`.
// //  */
// // vec3_t ctraj_get_angular_velocity(const ctraj_t &ctraj, const timestamp_t ts);
// //
// // /**
// //  * Save trajectory to file
// //  */
// // int ctraj_save(const ctraj_t &ctraj, const std::string &save_path);

/******************************************************************************
 *                               MEASUREMENTS
 *****************************************************************************/

// // // struct image_t : meas_t {
// // //   int width = 0;
// // //   int height = 0;
// // //   float *data = nullptr;
// // //
// // //   image_t() {}
// // //
// // //   image_t(const timestamp_t ts_, const int width_, const int height_)
// // //       : meas_t{ts_}, width{width_}, height{height_} {
// // //     data = new float[width * height];
// // //   }
// // //
// // //   image_t(const timestamp_t ts_,
// // //           const int width_,
// // //           const int height_,
// // //           float *data_)
// // //       : meas_t{ts_}, width{width_}, height{height_}, data{data_} {}
// // //
// // //   virtual ~image_t() {
// // //     if (data) {
// // //       free(data);
// // //     }
// // //   }
// // // };

// // /******************************************************************************
// //  *                                 MODELS
// //  *****************************************************************************/
// //
// // /**
// //  * Create DH transform from link n to link n-1 (end to front)
// //  *
// //  * @param[in] theta
// //  * @param[in] d
// //  * @param[in] a
// //  * @param[in] alpha
// //  *
// //  * @returns DH transform
// //  */
// // mat4_t dh_transform(const real_t theta,
// //                     const real_t d,
// //                     const real_t a,
// //                     const real_t alpha);
// //
// // /**
// //  * 2-DOF Gimbal Model
// //  */
// // struct gimbal_model_t {
// //   EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
// //
// //   // Parameter vector of transform from
// //   // static camera to base-mechanism
// //   vecx_t tau_s = zeros(6, 1);
// //
// //   // Parameter vector of transform from
// //   // end-effector to dynamic camera
// //   vecx_t tau_d = zeros(6, 1);
// //
// //   // First gibmal-joint
// //   real_t Lambda1 = 0.0;
// //   vec3_t w1 = zeros(3, 1);
// //
// //   // Second gibmal-joint
// //   real_t Lambda2 = 0.0;
// //   vec3_t w2 = zeros(3, 1);
// //
// //   real_t theta1_offset = 0.0;
// //   real_t theta2_offset = 0.0;
// //
// //   gimbal_model_t();
// //   gimbal_model_t(const vec6_t &tau_s,
// //                  const vec6_t &tau_d,
// //                  const real_t Lambda1,
// //                  const vec3_t w1,
// //                  const real_t Lambda2,
// //                  const vec3_t w2,
// //                  const real_t theta1_offset = 0.0,
// //                  const real_t theta2_offset = 0.0);
// //   virtual ~gimbal_model_t();
// // };
// //
// // /**
// //  * Set gimbal attitude
// //  *
// //  * @param[in,out] model Model
// //  * @param[in] roll Roll [rads]
// //  * @param[in] pitch Pitch [rads]
// //  */
// // void gimbal_model_set_attitude(gimbal_model_t &model,
// //                                const real_t roll,
// //                                const real_t pitch);
// //
// // /**
// //  * Get gimbal joint angle
// //  *
// //  * @param[in] model Model
// //  * @returns Gimbal joint angles
// //  */
// // vec2_t gimbal_model_get_joint_angles(const gimbal_model_t &model);
// //
// // /**
// //  * Returns transform from static camera to base mechanism
// //  *
// //  * @param[in] model Model
// //  * @returns Transform
// //  */
// // mat4_t gimbal_model_T_BS(const gimbal_model_t &model);
// //
// // /**
// //  * Returns transform from base mechanism to end-effector
// //  *
// //  * @param[in] model Model
// //  * @returns Transform
// //  */
// // mat4_t gimbal_model_T_EB(const gimbal_model_t &model);
// //
// // /**
// //  * Returns transform from end-effector to dynamic camera
// //  *
// //  * @param[in] model Model
// //  * @returns Transform
// //  */
// // mat4_t gimbal_model_T_DE(const gimbal_model_t &model);
// //
// // /**
// //  * Returns transform from static to dynamic camera
// //  *
// //  * @param[in] model Model
// //  * @returns Transform
// //  */
// // mat4_t gimbal_model_T_DS(const gimbal_model_t &model);
// //
// // /**
// //  * Returns transform from static to dynamic camera
// //  *
// //  * @param[in,out] model Model
// //  * @param[in] theta Gimbal roll and pitch [radians]
// //  * @returns Transform from static to dynamic camera
// //  */
// // mat4_t gimbal_model_T_DS(gimbal_model_t &model, const vec2_t &theta);
// //
// // /**
// //  * gimbal_model_t to output stream
// //  */
// // std::ostream &operator<<(std::ostream &os, const gimbal_model_t &gimbal);
// //
// // /**
// //  * Calculate target angular velocity and time taken to traverse a desired
// //  * circle * trajectory of radius r and velocity v
// //  *
// //  * @param[in] r Desired circle radius
// //  * @param[in] v Desired trajectory velocity
// //  * @param[in] w Target angular velocity
// //  * @param[in] time Target time taken to complete circle trajectory
// //  **/
// // void circle_trajectory(const real_t r, const real_t v, real_t *w, real_t *time);
// //
// // /**
// //  * Two wheel robot
// //  */
// // struct two_wheel_t {
// //   vec3_t r_G = vec3_t::Zero();
// //   vec3_t v_G = vec3_t::Zero();
// //   vec3_t a_G = vec3_t::Zero();
// //   vec3_t rpy_G = vec3_t::Zero();
// //   vec3_t w_G = vec3_t::Zero();
// //
// //   real_t vx_desired = 0.0;
// //   real_t yaw_desired = 0.0;
// //
// //   pid_t vx_controller{0.1, 0.0, 0.1};
// //   pid_t yaw_controller{0.1, 0.0, 0.1};
// //
// //   vec3_t a_B = vec3_t::Zero();
// //   vec3_t v_B = vec3_t::Zero();
// //   vec3_t w_B = vec3_t::Zero();
// //
// //   two_wheel_t() {}
// //
// //   two_wheel_t(const vec3_t &r_G_, const vec3_t &v_G_, const vec3_t &rpy_G_)
// //       : r_G{r_G_}, v_G{v_G_}, rpy_G{rpy_G_} {}
// //
// //   ~two_wheel_t() {}
// //
// //   void update(const real_t dt) {
// //     const vec3_t r_G_prev = r_G;
// //     const vec3_t v_G_prev = v_G;
// //     const vec3_t rpy_G_prev = rpy_G;
// //
// //     r_G += euler321(rpy_G) * v_B * dt;
// //     v_G = (r_G - r_G_prev) / dt;
// //     a_G = (v_G - v_G_prev) / dt;
// //
// //     rpy_G += euler321(rpy_G) * w_B * dt;
// //     w_G = rpy_G - rpy_G_prev;
// //     a_B = euler123(rpy_G) * a_G;
// //
// //     // Wrap angles to +/- pi
// //     for (int i = 0; i < 3; i++) {
// //       rpy_G(i) = (rpy_G(i) > M_PI) ? rpy_G(i) - 2 * M_PI : rpy_G(i);
// //       rpy_G(i) = (rpy_G(i) < -M_PI) ? rpy_G(i) + 2 * M_PI : rpy_G(i);
// //     }
// //   }
// // };
// //
// // /**
// //  * MAV model
// //  */
// // struct mav_model_t {
// //   vec3_t attitude{0.0, 0.0, 0.0};         ///< Attitude in global frame
// //   vec3_t angular_velocity{0.0, 0.0, 0.0}; ///< Angular velocity in global frame
// //   vec3_t position{0.0, 0.0, 0.0};         ///< Position in global frame
// //   vec3_t linear_velocity{0.0, 0.0, 0.0};  ///< Linear velocity in global frame
// //
// //   real_t Ix = 0.0963; ///< Moment of inertia in x-axis
// //   real_t Iy = 0.0963; ///< Moment of inertia in y-axis
// //   real_t Iz = 0.1927; ///< Moment of inertia in z-axis
// //
// //   real_t kr = 0.1; ///< Rotation drag constant
// //   real_t kt = 0.2; ///< Translation drag constant
// //
// //   real_t l = 0.9; ///< MAV arm length
// //   real_t d = 1.0; ///< drag constant
// //
// //   real_t m = 1.0;  ///< Mass
// //   real_t g = 9.81; ///< Gravity
// // };
// //
// // /**
// //  * Update
// //  *
// //  * @param[in,out] qm Model
// //  * @param[in] motor_inputs Motor inputs (m1, m2, m3, m4)
// //  * @param[in] dt Time difference (s)
// //  * @returns 0 for success, -1 for failure
// //  */
// // int mav_model_update(mav_model_t &qm,
// //                      const vec4_t &motor_inputs,
// //                      const real_t dt);
//
// // /******************************************************************************
// //  *                               PARAMETERS
// //  *****************************************************************************/
// //
// // struct imu_params_t {
// //   real_t rate = 0.0;        // IMU rate [Hz]
// //   real_t tau_a = 0.0;       // Reversion time constant for accel [s]
// //   real_t tau_g = 0.0;       // Reversion time constant for gyro [s]
// //   real_t sigma_g_c = 0.0;   // Gyro noise density [rad/s/sqrt(Hz)]
// //   real_t sigma_a_c = 0.0;   // Accel noise density [m/s^s/sqrt(Hz)]
// //   real_t sigma_gw_c = 0.0;  // Gyro drift noise density [rad/s^s/sqrt(Hz)]
// //   real_t sigma_aw_c = 0.0;  // Accel drift noise density [m/s^2/sqrt(Hz)]
// //   real_t g = 9.81;          // Gravity vector [ms-2]
// // };

} //  namespace proto
#endif // PROTO_CORE_HPP
