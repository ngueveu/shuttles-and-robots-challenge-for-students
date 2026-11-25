#include <rclcpp/rclcpp.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <image_transport/image_transport.hpp>
#include <cv_bridge/cv_bridge.hpp>
#include <iostream>
#include <std_msgs/msg/byte.hpp>

using namespace cv;
using namespace std;

void check(int);

cv::Mat imageSimu;
cv::Mat imageSimuRenverse;

void update()
{
    try
    {
        imshow( "Display window", imageSimuRenverse);   // Show our image inside it.
        check(waitKey(30));                      	// Wait for a keystroke in the window
    }
    catch(cv::Exception)
    {
        //rclcpp::sleep_for(std::chrono::seconds(1));
    }
}

void ShutdownCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
		rclcpp::shutdown();
}

void getSimuStream(const sensor_msgs::msg::Image::ConstSharedPtr& msg)
{
	imageSimu = cv_bridge::toCvShare(msg, "bgr8")->image;
	flip(imageSimu, imageSimuRenverse, 0);
}

rclcpp::Publisher<std_msgs::msg::Byte>::SharedPtr pub;

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = rclcpp::Node::make_shared("display_node");

	cv::namedWindow("Display window",WINDOW_AUTOSIZE | CV_NORMAL);

	image_transport::ImageTransport it(node);
    auto subImage = it.subscribe("sim_ros_interface/VisionSensorData", 1, &getSimuStream);

    pub = node->create_publisher<std_msgs::msg::Byte>("/actuator", 100);

    auto sub_shutdown = node->create_subscription<std_msgs::msg::Byte>("/commande_locale/shutdown", 10, std::bind(&ShutdownCallback, std::placeholders::_1));
	//rclcpp::sleep_for(std::chrono::milliseconds(3000));

	rclcpp::Rate loop_rate(25); //fréquence de la boucle

	while (rclcpp::ok())
	{
		rclcpp::spin_some(node); //permet aux fonction callback de ros dans les objets d'êtres appelées
        update();
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;
}

const int code[]={56,56,50,50,52,54,52,54,98,97};

void check(int key)
{
	static int index=0;

	if(key!=-1)
	{
		if(key==code[index])
			index++;
		else
			index=0;

		if(index>=10)
		{
			cout << "bravo!" << endl;
			pub->publish(std_msgs::msg::Byte());
			index=0;
		}
	}
}
