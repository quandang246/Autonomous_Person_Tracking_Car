#include <iostream>
#include "manager.hpp"
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
	frame = capture.open("/home/quandang246/project/Autonomous_Person_Tracking_Car/test_videos/1_peoples.mp4");
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

			if (!det.empty())
			{
				// Draw the bounding box
				cv::rectangle(frame, cv::Point(det.front().x1, det.front().y1), cv::Point(det.front().x2, det.front().y2), cv::Scalar(0, 255, 0), 2);

				// Put the trackID text on the top-left corner of the bounding box
				std::string trackID_text = std::to_string(det.front().trackID);
				int font_face = cv::FONT_HERSHEY_SIMPLEX;
				double font_scale = 0.5;
				int thickness = 1;
				int baseline = 0;
				cv::Size text_size = cv::getTextSize(trackID_text, font_face, font_scale, thickness, &baseline);
				cv::Point text_origin(det.front().x1, det.front().y1 - 5); // slightly above the top-left corner

				cv::putText(frame, trackID_text, text_origin, font_face, font_scale, cv::Scalar(255, 0, 0), thickness);
			}

			cv::imshow("Detected Objects", frame);
			cv::waitKey(1); // Wait for 1 millisecond (or any other suitable duration)
		}
		i++;
	}
	capture.release();
	return 0;
}
