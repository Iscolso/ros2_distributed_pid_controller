from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    sensor_node = Node(
        package='mi_primer_robot',
        executable='sensor',
        name='sensor_node_sim'
    )

    motors_node = Node(
        package='mi_primer_robot',
        executable='motors',
        name='motors_node'
    )

    brain_node = Node(
        package='mi_primer_robot',
        executable='cerebro',
        name='brain_node',
        parameters=[{
            'kp':4.5,
            'ki':0.8,
            'kd':0.2,
            'setpoint':15.0
        }]
    )

    return LaunchDescription([
        sensor_node,
        motors_node,
        brain_node
    ])