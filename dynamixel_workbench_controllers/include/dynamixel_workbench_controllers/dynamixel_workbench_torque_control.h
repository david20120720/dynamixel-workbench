#ifndef DYNAMIXEL_WORKBENCH_TORQUE_CONTROL_H
#define DYNAMIXEL_WORKBENCH_TORQUE_CONTROL_H

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

// Default setting
#define BAUDRATE                    1000000
#define DEVICENAME                  "/dev/ttyUSB0"
#define PROTOCOL_VERSION            2.0

#define PAN_MOTOR                   0
#define TILT_MOTOR                  1

namespace dynamixel_workbench_torque_control
{
struct ReadData{
 std::vector<bool> dxl_bool_data;
 std::vector<uint16_t> dxl_int_data;
};

class DynamixelWorkbenchTorqueControl
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
  bool dxl_addparam_result_;
  int dxl_comm_result_;
  bool dxl_getdata_result_;
  dynamixel_tool::DynamixelTool *dxl_motor_;
  std::vector<dynamixel_tool::DynamixelTool *> dynamixel_;
  std::map<std::string, ReadData *> read_data_;
  std::string device_name_;
  std::string motor_model_;
  int motor_id_;
  float protocol_version_;
  float baud_rate_;
  uint32_t read_value_;

 public:
  DynamixelWorkbenchTorqueControl();
  ~DynamixelWorkbenchTorqueControl();
  bool dynamixelControlLoop(void);

 private:
  bool initDynamixelWorkbenchTorqueControl(void);
  bool shutdownDynamixelWorkbenchTorqueControl(void);
  bool initMotor(dynamixel_tool::DynamixelTool *dxl_motor, std::string motor_model, uint8_t motor_id, float protocol_version);
  bool writeDynamixelRegister(dynamixel::PacketHandler *packetHandler, uint8_t id, uint16_t addr, uint8_t length, uint32_t value);
  bool writeSyncDynamixel(uint16_t addr, uint8_t length, uint32_t pan_motor_value, uint32_t tilt_motor_value);
  bool readDynamixelRegister(dynamixel::PacketHandler *packetHandler, uint8_t id, uint16_t addr, uint8_t length, uint32_t *value);
  bool readSyncDynamixel(uint16_t addr, uint8_t length, ReadData *data);
  bool readTorque(void);
  bool readMoving(void);
  bool readGoalPosition(void);
  bool readGoalVelocity(void);
  bool readPresentPosition(void);
  bool readPresentVelocity(void);
  bool writeTorque(bool onoff);

  bool controlPanTiltMotor(dynamixel_workbench_msgs::SetPosition::Request &req,
                           dynamixel_workbench_msgs::SetPosition::Response &res);
};
}

#endif //DYNAMIXEL_WORKBENCH_TORQUE_CONTROL_H