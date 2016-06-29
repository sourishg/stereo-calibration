#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char const *argv[])
{
  cv::Mat R1, R2, P1, P2, Q;
  cv::Mat K1, K2, R;
  cv::Vec3d T;
  cv::Mat D1, D2;
  cv::Mat img1 = imread("../left10.png", CV_LOAD_IMAGE_COLOR);
  cv::Mat img2 = imread("../right10.png", CV_LOAD_IMAGE_COLOR);

  cv::FileStorage fs1("cam_stereo.yml", cv::FileStorage::READ);
  fs1["K1"] >> K1;
  fs1["K2"] >> K2;
  fs1["D1"] >> D1;
  fs1["D2"] >> D2;
  fs1["R"] >> R;
  fs1["T"] >> T;

  fs1["R1"] >> R1;
  fs1["R2"] >> R2;
  fs1["P1"] >> P1;
  fs1["P2"] >> P2;
  fs1["Q"] >> Q;

  cv::Mat lmapx, lmapy, rmapx, rmapy;
  cv::Mat imgU1, imgU2;

  cv::initUndistortRectifyMap(K1, D1, R1, P1, img1.size(), CV_32F, lmapx, lmapy);
  cv::initUndistortRectifyMap(K2, D2, R2, P2, img2.size(), CV_32F, rmapx, rmapy);
  cv::remap(img1, imgU1, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(img2, imgU2, rmapx, rmapy, cv::INTER_LINEAR);

  imshow("image1", imgU1);
  imshow("image2", imgU2);
  
  Mat imgU1G, imgU2G;
  cvtColor(imgU1, imgU1G, CV_BGR2GRAY);
  cvtColor(imgU2, imgU2G, CV_BGR2GRAY);
  
  imwrite("../left_rect10.pgm", imgU1G);
  imwrite("../right_rect10.pgm", imgU2G);
  
  int k = waitKey(0);
  if (k == ' ') {
    imwrite("../left_rect10.png", imgU1);
    imwrite("../right_rect10.png", imgU2);
  }
  return 0;
}
