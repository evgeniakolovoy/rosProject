import rclpy
from rclpy.node import Node
from custom_interfaces.msg import RobotCommand #from <package>.msg import <MessageType>

class ControllerPublisher(Node): #this is a publisher node class

    def __init__(self):
        super().__init__("ControllerPublisher")

    # to-do : Create a publisher

    # self.publisher_ = self.create_publisher(MessageType, 'topic_name', queue size)
    #Queue size is usually 10
    #for message type, look for hints in this file
    #you can choose the topic name as long as it matches the subscriber

        self.input_command()

    def input_command(self):
        while rclpy.ok():
            try:
                user_input = input("Enter Command '1' to go forward\nEnter Command '2' to go backwards\n").strip()
                msg = RobotCommand()

                if user_input == "1":
                    msg.command = RobotCommand.MOVE_FORWARD
                elif user_input == "2":
                    msg.command = RobotCommand.MOVE_BACKWARDS
                else:
                    continue

                self.publisher_.publish(msg)
                self.get_logger().info(f"Message Published: {msg.command}")

            except KeyboardInterrupt:
                break

def main(args=None):
    rclpy.init(args=args)
    controller = ControllerPublisher()
    rclpy.spin(controller)
    rclpy.shutdown()
