#ifndef PROTO_CORE_CONFIG_HPP
#define PROTO_CORE_CONFIG_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include <yaml-cpp/yaml.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "proto/core/file.hpp"
#include "proto/core/log.hpp"
#include "proto/core/math.hpp"

namespace proto {

struct config_t {
  std::string file_path;
  YAML::Node root;
  bool ok = false;

  config_t();
  config_t(const std::string &file_path_);
  ~config_t();
};

/**
 * Load YAML file.
 * @returns 0 for success or -1 for failure.
 */
int yaml_load_file(const std::string file_path, YAML::Node &root);

/**
 * Get YAML node containing the parameter value.
 * @returns 0 for success or -1 for failure.
 */
int yaml_get_node(const config_t &config,
                  const std::string &key,
                  const bool optional,
                  YAML::Node &node);

/**
 * Check if yaml file has `key`.
 * @returns 0 for success or -1 for failure.
 */
int yaml_has_key(const config_t &config, const std::string &key);

/**
 * Check if yaml file has `key`.
 * @returns 0 for success or -1 for failure.
 */
int yaml_has_key(const std::string &file_path, const std::string &key);

/**
 * Check size of vector in config file and returns the size.
 */
template <typename T>
size_t yaml_check_vector(const YAML::Node &node,
                         const std::string &key,
                         const bool optional);

/**
 * Check matrix fields.
 */
void yaml_check_matrix_fields(const YAML::Node &node,
                              const std::string &key,
                              size_t &rows,
                              size_t &cols);

/**
 * Check matrix to make sure that the parameter has the data field "rows",
 * "cols" and "data". It also checks to make sure the number of values is the
 * same size as the matrix.
 */
template <typename T>
void yaml_check_matrix(const YAML::Node &node,
                       const std::string &key,
                       const bool optional,
                       size_t &rows,
                       size_t &cols);
template <typename T>
void yaml_check_matrix(const YAML::Node &node,
                       const std::string &key,
                       const bool optional);

template <typename T>
void parse(const config_t &config,
           const std::string &key,
           T &out,
           const bool optional = false);

template <typename T>
void parse(const config_t &config,
           const std::string &key,
           std::vector<T> &out,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           vec2_t &vec,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           vec3_t &vec,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           vec4_t &vec,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           vecx_t &vec,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           mat2_t &mat,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           mat3_t &mat,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           mat4_t &mat,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           matx_t &mat,
           const bool optional = false);
void parse(const config_t &config,
           const std::string &key,
           cv::Mat &mat,
           const bool optional = false);
template <typename T>
void parse(const config_t &config,
           const std::string &key,
           const int rows,
           const int cols,
           T &mat,
           const bool optional = false);

} //  namespace proto
#include "proto/core/config_impl.hpp"
#endif // PROTO_CORE_CONFIG_HPP