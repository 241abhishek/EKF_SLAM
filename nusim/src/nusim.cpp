#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/u_int64.hpp"

using namespace std::chrono_literals;

class NuSim : public rclcpp::Node
{
  public:
    NuSim()
    : Node("nusim"), timer_count_(0)
    {
      auto timer_param_desc = rcl_interfaces::msg::ParameterDescriptor{};
      timer_param_desc.description = "Timer frequency";
      declare_parameter("rate", 200.0, timer_param_desc);
      double timer_rate = get_parameter("rate").as_double();
      std::chrono::milliseconds rate = (std::chrono::milliseconds) ((int)(1000.0 / timer_rate));

      timestep_publisher_ = create_publisher<std_msgs::msg::UInt64>("~/timestep", 10);
      timer_ = create_wall_timer(
      rate, std::bind(&NuSim::timer_callback, this));
    }

  private:
    void timer_callback()
    {   
        //publish the current timestep
        auto message = std_msgs::msg::UInt64();
        message.data = timer_count_;
        timestep_publisher_->publish(message);
        timer_count_++;
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::UInt64>::SharedPtr timestep_publisher_;
    size_t timer_count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<NuSim>());
  rclcpp::shutdown();
  return 0;
}