#include <chrono>
#include "input_handler/input_handler_node.hpp"

using namespace std::chrono_literals;

InputHandlerNode::InputHandlerNode() : Node("input_handler_node") {
    using namespace std::placeholders;

    // to-do: Create a subscriber and store it in subscriber_
    // Message type and topic name must match the publisher
    // Callback function runs when a message is received
    // Look for the callback function in this file
    //
    // subscriber_ = this->create_subscription<MessageType>(
    //     "topic_name",
    //     queue_size,
    //     std::bind(&InputHandlerNode::callback_name, this, _1)
    // );

    // to-do: Create an action client
    // Look for the custom action type used in this file
    // The action name must match the action server
    //
    // action_client_ = rclcpp_action::create_client<ActionType>(
    //     this,
    //     "action_name"
    // );

    RCLCPP_INFO(this->get_logger(), "Input Handler Node has been started.");
}

void InputHandlerNode::robot_command_callback(const RobotCommand::SharedPtr msg) {
    if (movement_in_progress_) {
        RCLCPP_WARN(this->get_logger(), "Movement already in progress, ignoring new command.");
        return;
    }

    switch (msg->command) {
        case RobotCommand::MOVE_FORWARD:
            RCLCPP_INFO(this->get_logger(), "Received command: MOVE_FORWARD");
            send_move_request(1.0);
            break;
        case RobotCommand::MOVE_BACKWARDS:
            RCLCPP_INFO(this->get_logger(), "Received command: MOVE_BACKWARDS");
            send_move_request(-1.0);
            break;
        default:
            RCLCPP_WARN(this->get_logger(), "Received unknown command: %d", msg->command);
            break;
    }
}

void InputHandlerNode::send_move_request(double distance) {
    using namespace std::placeholders;

    if (!action_client_->wait_for_action_server(5s)) {
        RCLCPP_ERROR(this->get_logger(), "Action server not available after waiting");
        return;
    }

    movement_in_progress_ = true;

    auto goal_msg = MoveRobot::Goal();
    goal_msg.distance = distance;

    auto send_goal_options = rclcpp_action::Client<MoveRobot>::SendGoalOptions();
    send_goal_options.goal_response_callback = std::bind(&InputHandlerNode::goal_response_callback, this, _1);
    send_goal_options.result_callback = std::bind(&InputHandlerNode::result_callback, this, _1);

    action_client_->async_send_goal(goal_msg, send_goal_options);
}

void InputHandlerNode::goal_response_callback(typename GoalHandleMoveRobot::SharedPtr goal_handle) {
    if (!goal_handle) {
        RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server");
    } else {
        RCLCPP_INFO(this->get_logger(), "Goal accepted by server, waiting for result");
    }
}

void InputHandlerNode::result_callback(const GoalHandleMoveRobot::WrappedResult &result) {
    movement_in_progress_ = false;
    
    switch (result.code) {
        case rclcpp_action::ResultCode::SUCCEEDED:
            if (result.result->movement_succeeded) {
                RCLCPP_INFO(this->get_logger(), "Move succeeded");
            } else {
                RCLCPP_WARN(this->get_logger(), "Move failed");
            }
            break;
        case rclcpp_action::ResultCode::ABORTED:
            RCLCPP_ERROR(this->get_logger(), "Move was aborted");
            break;
        case rclcpp_action::ResultCode::CANCELED:
            RCLCPP_ERROR(this->get_logger(), "Move was canceled");
            break;
        default:
            RCLCPP_ERROR(this->get_logger(), "Unknown result code");
            break;
    }
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<InputHandlerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
