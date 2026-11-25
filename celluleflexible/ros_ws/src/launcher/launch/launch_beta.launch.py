from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression
from launch_ros.actions import Node

def generate_launch_description():
    
    # 1. Déclaration de l'argument (nbRobot)
    nb_robot_arg = DeclareLaunchArgument(
        'nbRobot',
        default_value='4',
        description='Nombre de robots de la simu'
    )
    nb_robot_config = LaunchConfiguration('nbRobot')

    # 2. Définition des nœuds permanents
    commande_node = Node(
        package='commande',
        executable='run',
        name='commande',
        output='screen',
        prefix=["xterm -geometry '80x20+550+0' -e "], # N'oubliez pas l'espace après le -e !
        arguments=[nb_robot_config]
    )

    aiguillage_node = Node(
        package='aiguillages',
        executable='run_Aiguillage',
        name='aiguillage',
        output='screen',
        prefix=["xterm -geometry '80x20+1100+0' -e "]

    )

    robot1_node = Node(
        package='robots',
        executable='robot',
        name='Robot1',
        output='screen',
        prefix=["xterm -geometry '80x20+0+315' -e "],
        arguments=['1']
    )

    robot2_node = Node(
        package='robots',
        executable='robot',
        name='Robot2',
        output='screen',
        prefix=["xterm -geometry '80x20+550+315' -e "],
        arguments=['2']
    )

    # 3. La condition pour les robots 3 et 4
    # On évalue si nbRobot est égal à la chaîne de caractères '4'
    condition_4_robots = IfCondition(
        PythonExpression(["'", nb_robot_config, "' == '4'"])
    )

    robot3_node = Node(
        package='robots',
        executable='robot',
        name='Robot3',
        output='screen',
        prefix=["xterm -geometry '80x20+0+607' -e "],
        arguments=['3'],
        condition=condition_4_robots # S'exécute uniquement si la condition est vraie
    )

    robot4_node = Node(
        package='robots',
        executable='robot',
        name='Robot4',
        output='screen',
        prefix=["xterm -geometry '80x20+550+607' -e "],
        arguments=['4'],
        condition=condition_4_robots # S'exécute uniquement si la condition est vraie
    )

    # 4. Nœuds de gestion
    log_manager_node = Node(
        package='commande_locale',
        executable='log_manager',
        name='log_manager'
    )

    shuttle_manager_node = Node(
        package='shuttles',
        executable='shuttle_manager',
        name='shuttle_manager',
        output='screen',
        prefix=["xterm -geometry '60x20+400+800' -e "]
    )
    
    # Nœud commenté d'origine :
    # shuttle_manager_node_commented = Node(
    #     package='shuttles',
    #     executable='main_ShuttleManager',
    #     name='shuttleManager'
    # )

    # 5. On rassemble tout pour l'exécution
    return LaunchDescription([
        nb_robot_arg,
        commande_node,
        aiguillage_node,
        robot1_node,
        robot2_node,
        robot3_node,
        robot4_node,
        log_manager_node,
        shuttle_manager_node
    ])
