#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int board_w = 9;
int board_h = 6;
float squareSize = 0.02423; //0.02423m

cv::Size board_sz = cv::Size(board_w, board_h);
int board_n = board_w * board_h;

vector<vector<cv::Point3f> > object_points;
vector<vector<cv::Point2f> > imagePoints1;
vector<cv::Point2f> corners1;
vector< vector< Point2f > > left_img_points;

Mat img1, gray1, spl1;

void load_image_points() {
  int num_imgs = 28;

  spl1 = imread("/home/sourish/pinhole_stereo/build/calib_imgs/2/right8.jpg", CV_LOAD_IMAGE_COLOR);

  /*
  Fimg1 = imread("/Users/sourishghosh/vision/FisheyeStereo/build/left.jpg", CV_LOAD_IMAGE_COLOR);
  Fimg2 = imread("/Users/sourishghosh/vision/FisheyeStereo/build/right.jpg", CV_LOAD_IMAGE_COLOR);
  */

  for (int i = 1; i <= num_imgs; i++) {
    char left_img[100];
    sprintf(left_img, "/home/sourish/pinhole_stereo/build/calib_imgs/2/right%d.jpg", i);
    img1 = imread(left_img, CV_LOAD_IMAGE_COLOR);
    cv::cvtColor(img1, gray1, CV_BGR2GRAY);

    bool found1 = false;

    found1 = cv::findChessboardCorners(img1, board_sz, corners1,
  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if (found1)
    {
      cv::cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1),
  cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
      cv::drawChessboardCorners(gray1, board_sz, corners1, found1);
    }
    
    vector<cv::Point3f> obj;
    for( int i = 0; i < board_h; ++i )
      for( int j = 0; j < board_w; ++j )
        obj.push_back(Point3d(double( (float)j * squareSize ), double( (float)i * squareSize ), 0));

    if (found1) {
      cout << i << ". Found corners!" << endl;
      imagePoints1.push_back(corners1);
      object_points.push_back(obj);
    }
  }
  for (int i = 0; i < imagePoints1.size(); i++) {
    vector< Point2f > v1, v2;
    for (int j = 0; j < imagePoints1[i].size(); j++) {
      v1.push_back(Point2f((double)imagePoints1[i][j].x, (double)imagePoints1[i][j].y));
    }
    left_img_points.push_back(v1);
  }
}

int main(int argc, char const *argv[])
{
  load_image_points();

  printf("Starting Calibration\n");
  cv::Mat K1;
  cv::Mat D1;
  vector< Mat > rvecs, tvecs;
  int flag = 0;
  flag |= CV_CALIB_FIX_K4;
  flag |= CV_CALIB_FIX_K5;
  calibrateCamera(object_points, left_img_points, spl1.size(), K1, D1, rvecs, tvecs, flag);

  cv::FileStorage fs1("cam_right.yml", cv::FileStorage::WRITE);
  fs1 << "K1" << K1;
  fs1 << "D1" << D1;
  printf("Done Calibration\n");

  return 0;
}
