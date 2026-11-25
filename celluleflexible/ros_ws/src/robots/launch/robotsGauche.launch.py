from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    robot1 = Node(
        package='robots',
        executable='robot',
        name='Robot1',
        output='screen',
        arguments=['1'],
        prefix="xterm -geometry 80x20+0+315 -e"
    )

    robot2 = Node(
        package='robots',
        executable='robot',
        name='Robot2',
        output='screen',
        arguments=['2'],
        prefix="xterm -geometry 80x20+550+315 -e"
    )

    return LaunchDescription([
        robot1,
        robot2
    ])

