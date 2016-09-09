#ifndef DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H
#define DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float64.h>
#include <dynamixel_workbench_toolbox/dynamixel_tool.h>
#include <dynamixel_workbench_msgs/MotorStateList.h>
#include <dynamixel_workbench_msgs/SetPosition.h>

#include <dynamixel_sdk/dynamixel_sdk.h>

#define PI 3.14159265358979323846
#define DEGREE2RADIAN (PI / 180.0)
#define RADIAN2DEGREE (180.0 / PI)

#define PAN_TILT_MOTOR 0
#define PAN_MOTOR      0
#define TILT_MOTOR     1

#define VELOCITY      100
#define ACCELERATION  20

namespace dynamixel_workbench_position_control
{
class DynamixelWorkbenchPositionControl
{
 public:
  dynamixel::PortHandler *portHandler_;
  dynamixel::PacketHandler *packetHandler_;

 private:
  // ROS NodeHandle
  ros::NodeHandle nh_;
  ros::NodeHandle nh_priv_;
  // ROS Parameters
  bool is_debug_;
  // ROS Topic Publisher
  ros::Publisher dxl_state_pub_;
  // ROS Service Server
  ros::ServiceServer position_control_server;
  // Parameters
  std::vector<dynamixel_tool::DynamixelTool *> dynamixel_;

  std::string device_name_;
  std::string motor_model_;
  int motor_id_;
  float protocol_version_;
  float baud_rate_;

  std::map<std::string, std::vector<int64_t> *> read_data_;
  int64_t read_value_;

 public:
  DynamixelWorkbenchPositionControl();
  ~DynamixelWorkbenchPositionControl();
  bool dynamixelControlLoop(void);

 private:
  bool initDynamixelWorkbenchPositionControl(void);
  bool shutdownDynamixelWorkbenchPositionControl(void);

  bool initMotor(std::string motor_model, uint8_t motor_id, float protocol_version);

  bool readDynamixelRegister(uint8_t id, uint16_t addr, uint8_t length, int64_t *value);
  bool readMotorState(std::string addr_name);

  bool writeSyncDynamixel(uint16_t addr, uint8_t length, int64_t pan_motor_value, int64_t tilt_motor_value);
  bool writeTorque(bool onoff);
  bool writeProfile();
  bool writePosition(int64_t pan_pos, int64_t tilt_pos);

  int64_t convertRadian2Value(double radian);

  bool getPublishedMsg(void);
  bool controlPanTiltMotorCallback(dynamixel_workbench_msgs::SetPosition::Request &req,
                                   dynamixel_workbench_msgs::SetPosition::Response &res);
};
}

#endif //DYNAMIXEL_WORKBENCH_POSITION_CONTROL_H