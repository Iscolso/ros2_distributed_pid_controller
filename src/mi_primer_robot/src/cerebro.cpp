#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using std::placeholders::_1;


class PIDController {
  public:
    PIDController (double kp, double ki, double kd)
      : kp_(kp), ki_(ki), kd_(kd), integral_(0.0), prev_error_(0.0) {}

    double calculate(double setpoint_, double current_value){
      double error = setpoint_ - current_value;
      integral_ += error;
      double derivative = error - prev_error_;
      prev_error_ = error;

      return (kp_* error) + (ki_ * integral_) + (kd_ * derivative);
    }

  private:
    double kp_, ki_, kd_, integral_, prev_error_;
};

class BrainNode : public rclcpp::Node {
  public:
    BrainNode() : Node("brain_node"), pid_(2.0, 0.5, 0.1), setpoint_(10.0) {
      RCLCPP_INFO(this->get_logger(), "Starting PID brain..., goal height: %.2f metros", setpoint_);

      publisher_ = this->create_publisher<std_msgs::msg::Float64>("/motors_force", 10);

      subscription_ = this->create_subscription<std_msgs::msg::Float64>("/initial_height", 10, std::bind(&BrainNode::callback_sensor, this, _1));
    }

  private:
    void callback_sensor(const std_msgs::msg::Float64 & msg_height) {
      double initial_height = msg_height.data;

      double force = pid_.calculate(setpoint_, initial_height);

      RCLCPP_INFO(this->get_logger(), "Height: %.2f | Calculated force: %.2f", initial_height, force);

      auto msg_comando = std_msgs::msg::Float64();
      msg_comando.data = force;
      
      publisher_->publish(msg_comando);
    }

    rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
    
    PIDController pid_;
    double setpoint_;
};

int main(int argc, char **argv){
  rclcpp::init(argc, argv);
  auto node = std::make_shared<BrainNode>();

  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}