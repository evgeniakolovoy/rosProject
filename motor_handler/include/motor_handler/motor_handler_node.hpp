#pragma once

#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include <custom_interfaces/action/move_robot.hpp>

class MotorHandlerNode : public rclcpp::Node {
public:
    using MoveRobot = custom_interfaces::action::MoveRobot;
    using GoalHandleMoveRobot = rclcpp_action::ServerGoalHandle<MoveRobot>;

    MotorHandlerNode();

private:
    rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID&, std::shared_ptr<const MoveRobot::Goal> goal);
    rclcpp_action::CancelResponse handle_cancel(const std::shared_ptr<GoalHandleMoveRobot>);
    void handle_accepted(const std::shared_ptr<GoalHandleMoveRobot> goal_handle);

    void update_position();

private:
    rclcpp_action::Server<MoveRobot>::SharedPtr action_server_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::shared_ptr<GoalHandleMoveRobot> curr_goal_handle_;

    double curr_position_ = 0.0;
    double new_position_ = 0.0;
    bool movement_in_progress_ = false;
};
