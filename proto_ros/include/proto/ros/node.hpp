#ifndef PROTO_ROS_NODE_HPP
#define PROTO_ROS_NODE_HPP

#include <functional>

#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/CameraInfo.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>

namespace proto {

#define ROS_GET_PARAM(X, Y)                                                    \
  if (ros_nh_ == NULL) {                                                       \
    ROS_ERROR("You did not do ros_node_t::configure() first!");                \
    ROS_ERROR("Can only call ROS_GET_PARAM() after configure!");               \
    return -1;                                                                 \
  }                                                                            \
  if (ros_nh_->getParam(X, Y) == false) {                                      \
    ROS_ERROR("Failed to get ROS param [%s]!", (X).c_str());                   \
    return -1;                                                                 \
  }

#define ROS_GET_OPTIONAL_PARAM(X, Y, DEFAULT_VALUE)													   \
  if (ros_nh_ == NULL) {                                                       \
    ROS_ERROR("You did not do ros_node_t::configure() first!");                \
    ROS_ERROR("Can only call ROS_GET_PARAM() after configure!");               \
    return -1;                                                                 \
  }                                                                            \
  if (ros_nh_->getParam(X, Y) == false) {                                      \
    ROS_INFO("ROS param [%s] not found, setting defaults!", (X).c_str());      \
		Y = DEFAULT_VALUE;																											   \
  }

#define RUN_ROS_NODE(NODE_CLASS)                                               \
  int main(int argc, char **argv) {                                            \
    NODE_CLASS node(argc, argv);                                               \
    if (node.configure() != 0) {                                               \
      ROS_ERROR("Failed to configure [%s]!", #NODE_CLASS);                     \
      return -1;                                                               \
    }                                                                          \
    node.loop();                                                               \
    return 0;                                                                  \
  }

#define RUN_ROS_NODE_RATE(NODE_CLASS, NODE_RATE)                               \
  int main(int argc, char **argv) {                                            \
    NODE_CLASS node(argc, argv);                                               \
    if (node.configure(NODE_RATE) != 0) {                                      \
      ROS_ERROR("Failed to configure [%s]!", #NODE_CLASS);                     \
      return -1;                                                               \
    }                                                                          \
    node.loop();                                                               \
    return 0;                                                                  \
  }

struct ros_node_t {
  bool configured_ = false;
  bool debug_mode_ = false;
  bool sim_mode_ = false;

  int argc_ = 0;
  char **argv_ = nullptr;

  std::string node_name_;
  size_t ros_seq_ = 0;
  ros::NodeHandle *ros_nh_ = nullptr;
  ros::Rate *ros_rate_ = nullptr;
  ros::Time ros_last_updated_;

  std::map<std::string, ros::Publisher> ros_pubs_;
  std::map<std::string, ros::Subscriber> ros_subs_;
  std::map<std::string, ros::ServiceServer> ros_services_;
  std::map<std::string, ros::ServiceClient> ros_clients_;

  std::map<std::string, image_transport::Publisher> img_pubs_;
  std::map<std::string, image_transport::Subscriber> img_subs_;
  std::function<int()> loop_cb_;

  ros_node_t();
  ros_node_t(int argc, char **argv);
  ~ros_node_t();

  int configure();
  int configure(const int hz);
  void shutdown_callback(const std_msgs::Bool &msg);
  int add_shutdown_subscriber(const std::string &topic);
  int add_image_publisher(const std::string &topic);
  int add_loop_callback(std::function<int()> cb);

  template <typename M, typename T>
  int add_image_subscriber(const std::string &topic,
                           void (T::*fp)(M),
                           T *obj,
                           uint32_t queue_size = 1) {
    // pre-check
    if (configured_ == false) {
      return -1;
    }

    // image transport
    image_transport::ImageTransport it(*ros_nh_);
    img_subs_[topic] = it.subscribe(topic, queue_size, fp, obj);

    return 0;
  }

  template <typename M>
  int add_publisher(const std::string &topic,
                    uint32_t queue_size = 1,
                    bool latch = false) {
    ros::Publisher publisher;

    // pre-check
    if (configured_ == false) {
      return -1;
    }

    // add publisher
    publisher = ros_nh_->advertise<M>(topic, queue_size, latch);
    ros_pubs_[topic] = publisher;

    return 0;
  }

  template <typename M, typename T>
  int add_subscriber(const std::string &topic,
                     void (T::*fp)(M),
                     T *obj,
                     uint32_t queue_size = 1) {
    ros::Subscriber subscriber;

    // pre-check
    if (configured_ == false) {
      return -1;
    }

    // add subscriber
    subscriber = ros_nh_->subscribe(topic, queue_size, fp, obj);
    ros_subs_[topic] = subscriber;

    return 0;
  }

  template <class T, class MReq, class MRes>
  int add_service(const std::string &service_topic,
                  bool (T::*fp)(MReq &, MRes &),
                  T *obj) {
    ros::ServiceServer server;

    // pre-check
    if (configured_ == false) {
      return -1;
    }

    // register service server
    server = ros_nh_->advertiseService(service_topic, fp, obj);
    ros_services_[service_topic] = server;

    return 0;
  }

  template <typename M>
  int addClient(const std::string &service_topic) {
    ros::ServiceClient client;

    // pre-check
    if (configured_ == false) {
      return -1;
    }

    // register service server
    client = ros_nh_->serviceClient<M>(service_topic);
    ros_clients_[service_topic] = client;

    return 0;
  }

  int loop();
};

} // namespace proto
#endif  // PROTO_ROS_NODE_HPP