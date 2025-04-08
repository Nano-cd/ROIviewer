#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include <opencv2/opencv.hpp>

using namespace cv;
VideoCapture g_cap;
Mat g_frame;
int g_verticalPos = 0;
int g_horizontalPos = 0;

void onTrackbar(int, void*) {
	Mat displayImg = g_frame.clone();

	// 保持原有的图形绘制逻辑
	rectangle(displayImg, Point(480, 380), Point(1450, 1300), Scalar(255, 255, 0), 2); // 矩形框
	rectangle(displayImg, Point(1580, 380), Point(2550, 1300), Scalar(255, 255, 0), 2); // 矩形框
	line(displayImg, Point(g_verticalPos, 0), Point(g_verticalPos, g_frame.rows), Scalar(0, 0, 255), 1);
	line(displayImg, Point(0, g_horizontalPos), Point(g_frame.cols, g_horizontalPos), Scalar(0, 0, 255), 1);

	imshow("Camera Viewer", displayImg);
}

int main()
{
	// 初始化摄像头（0表示默认摄像头，如有多个摄像头可尝试1,2等）
	g_cap.open(2, CAP_DSHOW);
	if (!g_cap.isOpened()) {
		std::cout << "无法打开摄像头\n";
		return -1;
	}
	// 先设置编码格式（部分摄像头需要MJPG格式才能支持高分辨率）
	
	g_cap.set(CAP_PROP_FRAME_WIDTH, 3264);  // 设置宽度为1920
	g_cap.set(CAP_PROP_FRAME_HEIGHT, 2448); // 设置高度为1080
	g_cap.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
		// 必须添加延迟让摄像头完成配置
	Sleep(200); // Windows系统
	g_cap >> g_frame;

	// 设置分辨率后立即验证
	int actual_width = g_cap.get(CAP_PROP_FRAME_WIDTH);
	int actual_height = g_cap.get(CAP_PROP_FRAME_HEIGHT);
	std::cout << "实际分辨率: " << actual_width << "x" << actual_height << std::endl;

	// 如果设置失败则使用默认分辨率
	if (actual_width != 3264 || actual_height != 2448) {
		std::cout << "警告：摄像头不支持3264x2448，使用实际分辨率: "
			<< actual_width << "x" << actual_height << std::endl;
	}

	if (g_frame.empty()) return -1;
	// 创建显示窗口
	// 创建窗口时指定尺寸（使用AUTOSIZE属性）
	namedWindow("Camera Viewer", WINDOW_NORMAL);
	resizeWindow("Camera Viewer", g_frame.cols, g_frame.rows);

	// 创建轨迹条控件（需要先获取一帧确定尺寸）
	//g_cap >> g_frame;
	//createTrackbar("竖线位置", "Camera Viewer", &g_verticalPos, g_frame.cols, onTrackbar);
	//createTrackbar("横线位置", "Camera Viewer", &g_horizontalPos, g_frame.rows, onTrackbar);

	// 实时视频循环
	while (true) {
		g_cap >> g_frame;
		if (g_frame.empty()) break;

		onTrackbar(0, 0);

		// 按ESC退出
		if (waitKey(10) == 27) break;
	}

	g_cap.release();
	return 0;
}
