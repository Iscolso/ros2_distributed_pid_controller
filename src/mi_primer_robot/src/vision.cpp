#include <chrono>
#include "std_msgs/msg/float64.hpp"
#include "rclcpp/rclcpp.hpp"
#include <opencv2/opencv.hpp>

using namespace std::chrono_literals;

class VisionNode : public rclcpp::Node{
    public:
        VisionNode() : Node("vision_node"){
            publisher_ = this->create_publisher<std_msgs::msg::Float64>("/x_position",10);


            cap_.open("mandarina.mp4");
            if(!cap_.isOpened()){
                RCLCPP_ERROR(this->get_logger(), "mp4 file couldn't open");
            } else{
                RCLCPP_INFO(this->get_logger(),"video loaded, searching for tangerine...");
            }

            timer_ = this->create_wall_timer(33ms, std::bind(&VisionNode::process_frame,this));
        }
    private:
        void process_frame(){
            cv::Mat frame, hsv, mask;

            cap_ >> frame;

            if(frame.empty()){
                cap_.set(cv::CAP_PROP_POS_FRAMES, 0);
                return;
            }

            cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);


            cv::Scalar lower_orange(5, 100, 100);
            cv::Scalar upper_orange(25,255,255);
            cv::inRange(hsv, lower_orange, upper_orange, mask);

            cv::Moments m = cv::moments(mask);

            if (m.m00 > 0 ){
                double center_x = m.m10 / m.m00;

                auto message = std_msgs::msg::Float64();
                message.data = center_x;
                publisher_->publish(message);

                RCLCPP_INFO(this->get_logger(), "Tangerine detected in x: .%2f", center_x);
            }
        }
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr publisher_;
        rclcpp::TimerBase::SharedPtr timer_;
        cv::VideoCapture cap_;
};

int main(int argc, char **argv){
    rclcpp::init(argc,argv);
    auto node = std::make_shared<VisionNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();

    return 0;
}