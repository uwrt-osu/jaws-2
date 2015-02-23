#include "ros/ros.h"
#include "boost/asio.hpp"
#include "jaws_msgs/Thrusters.h"

class Arbotix
{
  private:
    ros::NodeHandle nh;
    ros::Subscriber sub;
    boost::asio::io_service i_o;
    boost::asio::serial_port s_p;
  public:
    Arbotix() : nh(), i_o(), s_p(i_o, "/dev/ttyACM0")
    {
      sub = nh.subscribe<jaws_msgs::Thrusters>("thrusters", 1, &Arbotix::callback, this);
    }
    void callback(const jaws_msgs::Thrusters::ConstPtr& thrusters)
    {
      unsigned char d[12];

      d[0] = '-';
      d[1] = thrusters->port_angle & 0xFF;
      d[2] = (thrusters->port_angle >> 8) & 0xFF;

      d[3] = thrusters->stbd_angle & 0xFF;
      d[4] = (thrusters->stbd_angle >> 8) & 0xFF;

      d[5] = thrusters->port_power & 0xFF;
      d[6] = (thrusters->port_power >> 8) & 0xFF;

      d[7] = thrusters->stbd_power & 0xFF;
      d[8] = (thrusters->stbd_power >> 8) & 0xFF;

      d[9] = thrusters->aft_power & 0xFF;
      d[10] = (thrusters->aft_power >> 8) & 0xFF;

      s_p.write_some(boost::asio::buffer(&d, 12));
    }
    void loop()
    {
      ros::spin();
    }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "arbotix_node");
  Arbotix arbotix;
  arbotix.loop();
}
