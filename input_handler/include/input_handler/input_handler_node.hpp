#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <custom_interfaces/msg/robot_command.hpp>
#include <custom_interfaces/action/move_robot.hpp>

class InputHandlerNode : public rclcpp::Node {
public:
    using RobotCommand = custom_interfaces::msg::RobotCommand;
    using MoveRobot = custom_interfaces::action::MoveRobot;
    using GoalHandleMoveRobot = rclcpp_action::ClientGoalHandle<MoveRobot>;

    InputHandlerNode();

private:
    void robot_command_callback(const RobotCommand::SharedPtr msg);

    void send_move_request(double distance);

    void goal_response_callback(typename GoalHandleMoveRobot::SharedPtr goal_handle);
    void result_callback(const GoalHandleMoveRobot::WrappedResult &result);

private:
    rclcpp::Subscription<RobotCommand>::SharedPtr subscriber_;
    rclcpp_action::Client<MoveRobot>::SharedPtr action_client_;
    bool movement_in_progress_ = false;
};
