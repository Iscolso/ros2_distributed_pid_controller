#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64.hpp"

using namespace std::chrono_literals;

class SensorNode : public rclcpp::Node{
    public:
        SensorNode() : Node("sensor_node"), initial_height_(0.0){
            RCLCPP_INFO(this->get_logger(), "Reading data...");

            publisher_ = this->create_publisher<std_msgs::msg::Float64>("/initial_height",10);

            timer_ = this->create_wall_timer(
                500ms, std::bind(&SensorNode::publish_height, this));  
        }
    private:
        void publish_height(){
            if (initial_height_ < 10.0){
                initial_height_ += 0.5;
            }

            auto message = std_msgs::msg::Float64();
            message.data = initial_height_;
            
            RCLCPP_INFO(this->get_logger(), "Publishing current height: %2.f", message.data);
            publisher_->publish(message);
        }
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        double initial_height_;
};

int main(int argc, char **argv){
    rclcpp::init(argc,argv);

    auto node = std::make_shared<SensorNode>();

    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}