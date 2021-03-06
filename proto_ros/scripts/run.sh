# set -e

# for FILE in `find . -not -path "./.git/*" -type f`
# do
#   echo "PROCESSING [$FILE]"
#   sed -i "s/PROTO/PROTO/g" $FILE
# done

# make
# cd ~/catkin_ws/
# catkin build proto_ros
# source "$HOME/catkin_ws/devel/setup.bash"
# roslaunch proto_ros bag2ds.launch
# roslaunch proto_ros camera.launch
# roslaunch proto_ros calib_validate_mono.launch
# roslaunch proto_ros drone_hunt.launch

# export GAZEBO_PLUGIN_PATH=${GAZEBO_PLUGIN_PATH}:$HOME/catkin_ws/devel/lib
# export GAZEBO_MODEL_PATH=${GAZEBO_MODEL_PATH}:$HOME/catkin_ws/src/proto/proto_ros/models
# roslaunch proto_ros calib_sim.launch
# roslaunch proto_ros mav_sim.launch

# python scripts/rosutils/bag2csv.py \
#   /data/ucl/191213-scan-0.bag \
#   /ucl_0/vrpn_client/estimated_odometry \
#   /tmp/nav_odometry.csv
  # /vrpn_client_node/ucl_0/pose \
  # /ucl_0/mavros/local_position/pose \
# python scripts/rosutils/show_depth.py

# python scripts/rosutils/bag2csv.py \
#   ~/Downloads/bags/OscillationTest.bag \
#   /ucl_0/vrpn_client/estimated_odometry \
#   /tmp/nav_odometry.csv

# python scripts/rosutils/aabm_debug.py ~/Downloads/bags/OscillationTest.bag
# python scripts/sysid/battery_model_id.py ~/downloads/hover_3.bag
# python scripts/sysid/battery_model_id.py /data/ucl/sysid/ucl_hover.bag
# python scripts/sysid/model_data_prep.py /data/ucl/sysid/pitch_1.bag
# octave scripts/sysid/model_sysid.m

# 2020-11-16-19-34-08.bag

# python scripts/rosutils/bag2csv.py \
#   /data/aabm/sysid/2020-11-16-19-14-19.bag \
#   /abm_ucl/vrpn_client/raw_transform \
#   /tmp/mav_tf-2020-11-16-19-14-19.csv

# cd scripts/sysid
# octave-cli plot_traj.m \
#   "/tmp/mav_traj-2020-11-16-19-14-19" \
#   "/tmp/mav_tf-2020-11-16-19-14-19.csv"

# gnuplot -persist scripts/rosutils/plots/nav_odometry.gpi
