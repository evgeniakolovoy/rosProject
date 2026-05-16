#include <chrono>
#include <thread>

#include "motor_handler/motor_handler_node.hpp"

using namespace std::chrono_literals;

MotorHandlerNode::MotorHandlerNode() : Node("motor_handler_node") {
    using namespace std::placeholders;

    // to-do: Create an action server
    // Look for the custom action type used in this file
    // The action name must match the action client
    // The server uses goal, cancel and accepted callbacks
    //
    // action_server_ = rclcpp_action::create_server<ActionType>(
    //     this,
    //     "action_name",
    //     goal_callback,
    //     cancel_callback,
    //     accepted_callback
    // );

        std::bind(&MotorHandlerNode::handle_goal, this, _1, _2),
        std::bind(&MotorHandlerNode::handle_cancel, this, _1),
        std::bind(&MotorHandlerNode::handle_accepted, this, _1));
}

rclcpp_action::GoalResponse MotorHandlerNode::handle_goal(const rclcpp_action::GoalUUID&, std::shared_ptr<const MoveRobot::Goal> goal) {
    RCLCPP_INFO(this->get_logger(), "Received goal request with distance: %.2f", goal->distance);
    return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
}

rclcpp_action::CancelResponse MotorHandlerNode::handle_cancel(const std::shared_ptr<GoalHandleMoveRobot>) {
    RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
    return rclcpp_action::CancelResponse::ACCEPT;
}

void MotorHandlerNode::handle_accepted(const std::shared_ptr<GoalHandleMoveRobot> goal_handle) {
    if (movement_in_progress_) {
        auto result = std::make_shared<MoveRobot::Result>();
        result->movement_succeeded = false;
        RCLCPP_WARN(this->get_logger(), "Already executing a goal, rejecting new goal");
        goal_handle->abort(result);
        return;
    }

    curr_goal_handle_ = goal_handle;
    new_position_ = curr_position_ + goal_handle->get_goal()->distance;
    movement_in_progress_ = true;

    //To-do: Create a timer
    //The timer repeatedly calls update_position()
    //This simulates the robot moving over time
    //The timer should run every 100 milliseconds
    //Look for the callback function used to update movement
    //
    //
    // timer_ = this->create_wall_timer(
    //     time_interval,
    //     std::bind(&MotorHandlerNode::callback_name, this)
    // );
}

void MotorHandlerNode::update_position() {
    if (!curr_goal_handle_) return;

    if (curr_goal_handle_->is_canceling()) {
        auto result = std::make_shared<MoveRobot::Result>();
        result->movement_succeeded = false;
        curr_goal_handle_->canceled(result);
        RCLCPP_INFO(this->get_logger(), "Goal canceled at position: %.2f", curr_position_);
        curr_goal_handle_.reset();
        timer_->cancel();
        movement_in_progress_ = false;
        return;
    }

    double step = (new_position_ > curr_position_) ? 0.1 : -0.1;
    curr_position_ += step;
    if ((step > 0 && curr_position_ > new_position_) ||
        (step < 0 && curr_position_ < new_position_)) 
    {
        curr_position_ = new_position_;
    }

    RCLCPP_INFO(this->get_logger(), "Current position: %.2f", curr_position_);

    if (curr_position_ == new_position_) {
        auto result = std::make_shared<MoveRobot::Result>();
        result->movement_succeeded = true;
        curr_goal_handle_->succeed(result);
        RCLCPP_INFO(this->get_logger(), "Goal succeeded, final position: %.2f", curr_position_);
        curr_goal_handle_.reset();
        timer_->cancel();
        movement_in_progress_ = false;
    }
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MotorHandlerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
