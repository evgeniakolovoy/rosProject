from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([
        Node(
            package='motor_handler',
            executable='motor_handler_node',
            name='motor_handler_node',
            output='screen'
        )
    ])
