#include <iostream>
#include <windows.h>
#include <commdlg.h>
#include <opencv2/opencv.hpp>

using namespace cv;

// 全局变量用于轨迹条回调
Mat g_img;
int g_verticalPos = 0;
int g_horizontalPos = 0;

void onTrackbar(int, void*) {
	Mat displayImg = g_img.clone();

	// 固定图形绘制（不受轨迹条影响）
	line(displayImg, Point(580, 0), Point(580, 800), Scalar(0, 255, 0), 2);    // 竖直线
	rectangle(displayImg, Point(480, 320), Point(1120, 540), Scalar(255, 255, 0), 2); // 矩形框
	line(displayImg, Point(0, 430), Point(1280, 430), Scalar(0, 255, 0), 2);   // 水平线

	// 动态参考线绘制（受轨迹条控制）
	line(displayImg, Point(g_verticalPos, 0), Point(g_verticalPos, g_img.rows), Scalar(0, 0, 255), 1);
	line(displayImg, Point(0, g_horizontalPos), Point(g_img.cols, g_horizontalPos), Scalar(0, 0, 255), 1);

	imshow("Image Viewer", displayImg);
}

int main()
{
	// 初始化文件选择对话框
	OPENFILENAMEA ofn;
	char szFile[10000] = { 0 };
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Image Files\0*.jpg;*.jpeg;*.png;*.bmp\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileNameA(&ofn)) {
		std::cout << "未选择文件\n";
		return 0;
	}

	// 读取图像文件
	g_img = imread(szFile);
	if (g_img.empty()) {
		std::cout << "无法加载图像\n";
		return -1;
	}

	// 创建显示窗口
	namedWindow("Image Viewer", WINDOW_AUTOSIZE);

	// 创建轨迹条控件
	createTrackbar("竖线位置", "Image Viewer", &g_verticalPos, g_img.cols, onTrackbar);
	createTrackbar("横线位置", "Image Viewer", &g_horizontalPos, g_img.rows, onTrackbar);

	// 初始显示
	onTrackbar(0, 0);
	waitKey(0);

	return 0;
}
