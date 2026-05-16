from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    return LaunchDescription([
        Node(
            package='input_handler',
            executable='input_handler_node',
            name='input_handler_node',
            output='screen'
        )
    ])
