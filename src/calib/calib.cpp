#include "prototype/calib/calib.hpp"

namespace prototype {

calib_target_t::calib_target_t() {}

calib_target_t::~calib_target_t() {}

std::ostream &operator<<(std::ostream &os, const calib_target_t &target) {
  os << "target_type: " << target.target_type << std::endl;
  os << "tag_rows: " << target.tag_rows << std::endl;
  os << "tag_cols: " << target.tag_cols << std::endl;
  os << "tag_size: " << target.tag_size << std::endl;
  os << "tag_spacing: " << target.tag_spacing << std::endl;
  return os;
}

int calib_target_load(calib_target_t &ct, const std::string &target_file) {
  config_t config{target_file};
  if (config.ok == false) {
    LOG_ERROR("Failed to load target file [%s]!", target_file.c_str());
    return -1;
  }
  parse(config, "target_type", ct.target_type);
  parse(config, "tag_rows", ct.tag_rows);
  parse(config, "tag_cols", ct.tag_cols);
  parse(config, "tag_size", ct.tag_size);
  parse(config, "tag_spacing", ct.tag_spacing);

  return 0;
}

int preprocess_camera_data(const calib_target_t &target,
                           const std::string &image_dir,
                           const vec2_t image_size,
                           const double lens_hfov,
                           const double lens_vfov,
                           const std::string &output_dir) {
  // Check image dir
  if (dir_exists(image_dir) == false) {
    LOG_ERROR("Image dir [%s] does not exist!", image_dir.c_str());
    return -1;
  }

  // Get image paths
  std::vector<std::string> image_paths;
  if (list_dir(image_dir, image_paths) != 0) {
    LOG_ERROR("Failed to traverse dir [%s]!", image_dir.c_str());
    return -1;
  }
  std::sort(image_paths.begin(), image_paths.end());

  // Setup AprilGrid detector
  // Setup camera intrinsics and distortion vector
  const double fx = pinhole_focal_length(image_size(0), lens_hfov);
  const double fy = pinhole_focal_length(image_size(1), lens_vfov);
  const double cx = image_size(0) / 2.0;
  const double cy = image_size(1) / 2.0;
  const mat3_t cam_K = pinhole_K(fx, fy, cx, cy);
  const vec4_t cam_D = zeros(4, 1);

  // Detect AprilGrid
  for (size_t i = 0; i < image_paths.size(); i++) {
    LOG_INFO("Processing image file: [%s]", image_paths[i].c_str());

    // Output file path
    auto output_file = basename(image_paths[i]);
    const long ts = std::stol(output_file);
    output_file = remove_ext(output_file);
    output_file += ".csv";
    const auto save_path = paths_combine(output_dir, output_file);

    // Detect
    const auto image_path = paths_combine(image_dir, image_paths[i]);
    const cv::Mat image = cv::imread(image_path);
    aprilgrid_t grid{ts,
                     target.tag_rows,
                     target.tag_cols,
                     target.tag_size,
                     target.tag_spacing};
    aprilgrid_detect(grid, image, cam_K, cam_D);

    // Save AprilGrid
    if (grid.detected) {
      aprilgrid_save(grid, save_path);
      aprilgrid_imshow(grid, "AprilGrid Detection", image);
    }
  }

  return 0;
}

int load_camera_calib_data(const std::string &data_dir,
                           std::vector<aprilgrid_t> &aprilgrids) {
  // Check image dir
  if (dir_exists(data_dir) == false) {
    LOG_ERROR("Image dir [%s] does not exist!", data_dir.c_str());
    return -1;
  }

  // Get detection data
  std::vector<std::string> data_paths;
  if (list_dir(data_dir, data_paths) != 0) {
    LOG_ERROR("Failed to traverse dir [%s]!", data_dir.c_str());
    return -1;
  }
  std::sort(data_paths.begin(), data_paths.end());

  // Load AprilGrid data
  for (size_t i = 0; i < data_paths.size(); i++) {
    LOG_INFO("Loading calib data: [%s]", data_paths[i].c_str());

    const auto data_path = paths_combine(data_dir, data_paths[i]);
    aprilgrid_t grid;
    if (aprilgrid_load(grid, data_path) != 0) {
      LOG_ERROR("Failed to load AprilGrid data [%s]!", data_path.c_str());
      return -1;
    }
    aprilgrids.emplace_back(grid);
  }

  return 0;
}

} //  namespace prototype