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

/*
int main()
{
	// Initalize car
	car my_car;

	// Define threshold frame width = 1280, height = 960
	int t_x1 = 620, t_y1 = 500, t_x2 = 660, t_y2 = 460;

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
	// Get the width and height of the frames the camera captures.
	double frame_width = capture.get(cv::CAP_PROP_FRAME_WIDTH);
	double frame_height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);

	// Print the frame size.
	std::cout << "Frame width: " << frame_width << std::endl;
	std::cout << "Frame height: " << frame_height << std::endl;

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
				if (mid_x < t_x1)
				{
					if (mid_y > t_y1)
					{
						my_car.diagonally_top_left();
					}
					else if (mid_y <= t_y1 && mid_y >= t_y2)
					{
						my_car.sideways_left();
					}
					else
					{
						my_car.diagonally_bottom_left();
					}
				}
				else if (mid_x >= t_x1 && mid_x <= t_x2)
				{
					if (mid_y > t_y1)
					{
						my_car.go_forward();
					}
					else if (mid_y <= t_y1 && mid_y >= t_y2)
					{
						my_car.refresh();
					}
					else
					{
						my_car.go_backward();
					}
				}
				else
				{
					if (mid_y > t_y1)
					{
						my_car.diagonally_top_right();
					}
					else if (mid_y <= t_y1 && mid_y >= t_y2)
					{
						my_car.sideways_right();
					}
					else
					{
						my_car.diagonally_bottom_right();
					}
				}
			}
			else
			{
				my_car.rotation();
			}

			cv::imshow("Detected Objects", frame);
			if (waitKey(10) == 27)
			{
				cout << "Esc key is pressed by user. Stoppig the video" << endl;
				break;
			}
		}
		i++;
	}
	capture.release();


	return 0;
}
*/

int main()
{
	car my_car;

	bool run = true;

	while (run)
	{
		int choice;

		std::cout << "Enter a number: ";
		std::cin >> choice;
		switch (choice)
		{
		case 1:
			my_car.go_forward();
			break;
		case 2:
			my_car.go_backward();
			break;
		case 3:
			my_car.sideways_left();
			break;
		case 4:
			my_car.sideways_right();
			break;
		case 5:
			my_car.diagonally_bottom_left();
			break;
		case 6:
			my_car.diagonally_bottom_right();
			break;
		case 7:
			my_car.diagonally_top_left();
			break;
		case 8:
			my_car.diagonally_top_right();
			break;
		case 9:
			my_car.rotation();
			break;
		case 10:
			run = false;
			break;
		default:
			std::cout << "Invalid choice. Please enter a number between 1 and 3." << std::endl;
			break;
		}
	}

	return 0;
}
