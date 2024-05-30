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
	int t_x = 630, t_y = 370, t_weight = 20, t_height = 20; 

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
	frame = capture.open("/home/quandang246/project/Autonomous_Person_Tracking_Car/test_videos/1_people_and_obstacle.mp4");
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
			std::cout << "FPS: "  << 1000.0/delay_infer << std::endl;

			if (!det.empty())
			{
				// Init val
				// Det box 
				float det_x = det.front().x1
				float det_y = det.front().y1
				float det_weight = det.front().x2
				float det_height = det.front().y2
				
				// Middle point
				float mid_x = det_x + det_weight / 2
				float mid_y = det_y - det_height / 2


				// Drawing things 
				// Draw the threshold box 
				cv::rectangle(frame, cv::Rect rect(t_x, t_y, t_weight, t_height), cv::Scalar(255, 0, 0))

				// Draw the bounding box
				cv::rectangle(frame, cv::Point(det_x, det_y), cv::Point(det_weight, det_height), cv::Scalar(0, 255, 0), 2);

				// Draw line
				cv::line(frame, cv::Point(mid_x, mid_y), cv::Point(t_x + t_weight/2, t_y - t_height /2), cv::Scalar(0, 0, 255), 2);

				// Put the trackID text on the top-left corner of the bounding box
				cv::Size text_size = cv::getTextSize(std::to_string(det.front().trackID), cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, 0);
				cv::Point text_origin(det_x, det_y - 5); // slightly above the top-left corner
				cv::putText(frame, trackID_text, text_origin, font_face, font_scale, cv::Scalar(255, 0, 0), thickness);
			
				// Control car 
				if (mid_x <= t_x)
				{
					my_car.turn_left();
				}
				else if (mid_x > t_x + t_weight)
				{
					my_car.turn_right();
				}
				if (mid_y >= t_y)
				{
					my_car.go_forward();
				}
				else if (mid_y < t_y)
				{
					my_car.go_backward();
				}

			}

			cv::imshow("Detected Objects", frame);
			cv::waitKey(1); // Wait for 1 millisecond (or any other suitable duration)
		}
		i++;
	}
	capture.release();
	
	return 0;
}
