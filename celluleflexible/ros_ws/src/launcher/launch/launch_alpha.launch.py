from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    
    # Équivalent de : <arg name="nbRobot" default="4" ... />
    nbRobot_arg = DeclareLaunchArgument(
        'nbRobot',
        default_value='4',
        description='Nombre de robots de la simu'
    )

    # Équivalent de : <node name="commande_locale" ... />
    commande_locale_node = Node(
        package='commande_locale',
        executable='simulation',
        name='commande_locale',
        output='screen',
        prefix=["xterm -geometry '80x20+0+0' -e "], # Le launch-prefix
        arguments=[LaunchConfiguration('nbRobot')]  # Les args
    )

    # Équivalent du nœud commenté :
    # display_node = Node(
    #     package='commande_locale',
    #     executable='display_node',
    #     name='display_node'
    # )

    return LaunchDescription([
        nbRobot_arg,
        commande_locale_node
        # display_node
    ])
