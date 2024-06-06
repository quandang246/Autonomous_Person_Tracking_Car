#include <iostream>
#include "manager.hpp"
#include "car_controller.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <vector>
#include <chrono>
#include <map>
#include <cmath>
#include <time.h>
using namespace cv;

int main()
{
	// Initalize car
	car my_car;

	// Define threshold frame width = 1280, height = 720
	int t_x1 = 630, t_y1 = 370, t_x2 = 650, t_y2 = 350;

	// calculate every person's (id,(up_num,down_num,average_x,average_y))
	map<int, vector<int>> personstate;
	map<int, int> classidmap;
	bool is_first = true;
	char *yolo_engine = "/home/quandang246/project/Autonomous_Person_Tracking_Car/resources/yolov5s.engine";
	char *sort_engine = "/home/quandang246/project/Autonomous_Person_Tracking_Car/resources/deepsort.engine";
	float conf_thre = 0.4;
	Trtyolosort yosort(yolo_engine, sort_engine);
	VideoCapture capture;
	cv::Mat frame;

	// Path to sample videos
	frame = capture.open("/home/quandang246/project/Autonomous_Person_Tracking_Car/test_videos/line.mp4");
	if (!capture.isOpened())
	{
		std::cout << "can not open" << std::endl;
		return -1;
	}
	capture.read(frame);
	std::vector<DetectBox> det;
	auto start_draw_time = std::chrono::system_clock::now();

	clock_t start_draw, end_draw;
	start_draw = clock();
	int i = 0;
	while (capture.read(frame))
	{
		if (i % 3 == 0)
		{
			// std::cout<<"origin img size:"<<frame.cols<<" "<<frame.rows<<std::endl;
			auto start = std::chrono::system_clock::now();
			yosort.TrtDetect(frame, conf_thre, det);
			auto end = std::chrono::system_clock::now();
			int delay_infer = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "delay_infer:" << delay_infer << "ms" << std::endl;
			std::cout << "FPS: " << 1000.0 / delay_infer << std::endl;

			if (!det.empty())
			{
				// Init val
				// Det box
				float det_x1 = det.front().x1;
				float det_y1 = det.front().y1;
				float det_x2 = det.front().x2;
				float det_y2 = det.front().y2;

				// Middle point
				float mid_x = det_x1 + (det_x2 - det_x1) / 2;
				float mid_y = det_y2 + (det_y1 - det_y2) / 2;

				// Drawing things
				// Draw the threshold box
				cv::rectangle(frame, cv::Point(t_x1, t_y1), cv::Point(t_x2, t_y2), cv::Scalar(255, 0, 0), 2);

				// Draw the bounding box
				cv::rectangle(frame, cv::Point(det_x1, det_y1), cv::Point(det_x2, det_y2), cv::Scalar(0, 255, 0), 2);

				// Draw line
				cv::line(frame, cv::Point(mid_x, mid_y), cv::Point(t_x1 + (t_x2 - t_x1) / 2, t_y2 + (t_y1 - t_y2) / 2), cv::Scalar(0, 0, 255), 2);

				// Put the trackID text on the top-left corner of the bounding box
				cv::Size text_size = cv::getTextSize(std::to_string(det.front().trackID), cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
				cv::Point text_origin(det_x1, det_y1 - 5); // slightly above the top-left corner
				cv::putText(frame, std::to_string(det.front().trackID), text_origin, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 0, 0), 1);

				// Control car
				if (mid_y > t_y1)
				{
					if (mid_x < t_x1)
					{
						my_car.turn_left();
					}
					else if (mid_x > t_x2)
					{
						my_car.turn_right();
					}
					else
					{
						my_car.go_forward();
					}
				}
				else if (mid_y < t_y2)
				{
					my_car.go_backward();
				}
				else
				{
					my_car.refresh();
				}
			}
			else 
			{
				my_car.refresh();
			}

			cv::imshow("Detected Objects", frame);
			cv::waitKey(1); // Wait for 1 millisecond (or any other suitable duration)
		}
		i++;
	}
	capture.release();

	return 0;
}
