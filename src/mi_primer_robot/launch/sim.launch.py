from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    vision_node = Node(
        package='mi_primer_robot',
        executable='vision',
        name='eyes_vision_opencv'
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
            'setpoint':320
        }]
    )

    return LaunchDescription([
        vision_node,
        motors_node,
        brain_node
    ])