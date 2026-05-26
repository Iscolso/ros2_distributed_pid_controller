#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using std::placeholders::_1;

class MotorsNode : public rclcpp::Node {
    public:
        MotorsNode() : Node("motors_node"){
            RCLCPP_INFO(this->get_logger(), "Starting motors and waiting for force...");

            subscription_ = this->create_subscription<std_msgs::msg::Float64>(
                "/motors_force", 10, std::bind(&MotorsNode::callback_motors, this, _1));
        }
    private:
        void callback_motors(const std_msgs::msg::Float64 & message) const{
            RCLCPP_INFO(this->get_logger(), "Force received: %.2f -> moving motors!", message.data);
        }
        rclcpp::Subscription<std_msgs::msg::Float64>::SharedPtr subscription_;
};

int main(int argc, char **argv){
    rclcpp::init(argc, argv);

    auto node = std::make_shared<MotorsNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}