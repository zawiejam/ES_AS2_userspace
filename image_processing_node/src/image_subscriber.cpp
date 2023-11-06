#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include "xprocess_image.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define IMAGE_SIZE IMAGE_WIDTH * IMAGE_HEIGHT

class ImageSubscriber : public rclcpp::Node
{
    public:
        ImageSubscriber() : Node("image_subscriber")
        {
            RCLCPP_INFO(this->get_logger(), "Initializing ImageSubscriber node");

            int status = XProcess_image_Initialize(&ip_inst, "process_image");
            if (status != XST_SUCCESS) {
                RCLCPP_INFO(this->get_logger(), "Error: Could not initialize the IP core.");
                return;
            }
            
            
            processed_image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("processed_image", 10);
            
            camera_subscription_ = this->create_subscription<sensor_msgs::msg::Image>(
                "/image_raw",
                10,
                std::bind(&ImageSubscriber::onImageMsg, this, std::placeholders::_1)
            );
        }

    private:
        rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr camera_subscription_;
        rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr processed_image_publisher_;
        XProcess_image ip_inst;

        void onImageMsg(const sensor_msgs::msg::Image::SharedPtr msg)
        {
            RCLCPP_INFO(this->get_logger(), "Received image");

            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, msg->encoding);
            cv::Mat img = cv_ptr->image;

            // Flatten
            std::vector<uint8_t> flattened_input_image;
            for (int row = 0; row < img.rows; row++) {
                for (int col = 0; col < img.cols; col++) {
                    flattened_input_image.push_back(img.at<uint8_t>(row, col));
                }
            }
            
            XProcess_image_Write_input_image_Words(&ip_inst, 0, (unsigned int *)flattened_input_image.data(), IMAGE_SIZE / 4);
            XProcess_image_Start(&ip_inst);

            // Wait for the IP core to finish
            while (!XProcess_image_IsDone(&ip_inst));
            
            std::vector<uint8_t> output_image_flat;
            output_image_flat.reserve(IMAGE_SIZE);
            XProcess_image_Read_output_image_Words(&ip_inst, 0, (unsigned int *)output_image_flat.data(), IMAGE_SIZE / 4);
            
            // Unflatten
            cv::Mat output_image_mat(IMAGE_HEIGHT, IMAGE_WIDTH, CV_8UC2);
            for(int i = 0; i < IMAGE_SIZE; i++) {
                output_image_mat.at<uint8_t>((int)(i / IMAGE_WIDTH), i % IMAGE_WIDTH) = output_image_flat[i];
            }
            
            cv_bridge::CvImage output_image_ros;
            output_image_ros.header = msg->header;
            output_image_ros.encoding = msg->encoding;
            output_image_ros.image = output_image_mat;

            processed_image_publisher_->publish(*(output_image_ros.toImageMsg()));

            RCLCPP_INFO(this->get_logger(), "Successfully processed image");
        }
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ImageSubscriber>());
    rclcpp::shutdown();
    return 0;
}
