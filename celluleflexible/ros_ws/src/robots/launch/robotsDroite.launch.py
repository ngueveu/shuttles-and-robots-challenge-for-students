from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    robot3 = Node(
        package='robots',
        executable='robot',
        name='Robot3',
        output='screen',
        arguments=['3'],
        prefix="xterm -geometry 80x20+0+607 -e"
    )

    robot4 = Node(
        package='robots',
        executable='robot',
        name='Robot4',
        output='screen',
        arguments=['4'],
        prefix="xterm -geometry 80x20+550+607 -e"
    )

    return LaunchDescription([
        robot3,
        robot4
    ])

