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
vector<vector<cv::Point2f> > imagePoints1, imagePoints2;
vector<cv::Point2f> corners1, corners2;
vector< vector< Point2f > > left_img_points, right_img_points;

Mat img1, img2, gray1, gray2, spl1, spl2, Fimg1, Fimg2;

void load_image_points() {
  int num_imgs = 28;

  spl1 = imread("/home/sourish/pinhole_stereo/build/calib_imgs/2/left5.jpg", CV_LOAD_IMAGE_COLOR);
  spl2 = imread("/home/sourish/pinhole_stereo/build/calib_imgs/2/right5.jpg", CV_LOAD_IMAGE_COLOR);

  for (int i = 1; i <= num_imgs; i++) {
    char left_img[100], right_img[100];
    sprintf(left_img, "/home/sourish/pinhole_stereo/build/calib_imgs/2/left%d.jpg", i);
    sprintf(right_img, "/home/sourish/pinhole_stereo/build/calib_imgs/2/right%d.jpg", i);
    img1 = imread(left_img, CV_LOAD_IMAGE_COLOR);
    img2 = imread(right_img, CV_LOAD_IMAGE_COLOR);
    cv::cvtColor(img1, gray1, CV_BGR2GRAY);
    cv::cvtColor(img2, gray2, CV_BGR2GRAY);

    bool found1 = false, found2 = false;

    found1 = cv::findChessboardCorners(img1, board_sz, corners1,
  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
    found2 = cv::findChessboardCorners(img2, board_sz, corners2,
  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    if (found1)
    {
      cv::cornerSubPix(gray1, corners1, cv::Size(5, 5), cv::Size(-1, -1),
  cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
      cv::drawChessboardCorners(gray1, board_sz, corners1, found1);
    }
    if (found2)
    {
      cv::cornerSubPix(gray2, corners2, cv::Size(5, 5), cv::Size(-1, -1),
  cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
      cv::drawChessboardCorners(gray2, board_sz, corners2, found2);
    }

    vector<cv::Point3f> obj;
    for( int i = 0; i < board_h; ++i )
      for( int j = 0; j < board_w; ++j )
        obj.push_back(Point3f(double( (float)j * squareSize ), double( (float)i * squareSize ), 0));

    if (found1 && found2) {
      cout << i << ". Found corners!" << endl;
      imagePoints1.push_back(corners1);
      imagePoints2.push_back(corners2);
      object_points.push_back(obj);
    }
  }
  for (int i = 0; i < imagePoints1.size(); i++) {
    vector< Point2f > v1, v2;
    for (int j = 0; j < imagePoints1[i].size(); j++) {
      v1.push_back(Point2f((double)imagePoints1[i][j].x, (double)imagePoints1[i][j].y));
      v2.push_back(Point2f((double)imagePoints2[i][j].x, (double)imagePoints2[i][j].y));
    }
    left_img_points.push_back(v1);
    right_img_points.push_back(v2);
  }
}

int main(int argc, char const *argv[])
{
  load_image_points();
  FileStorage fsl("cam_left.yml", FileStorage::READ);
  FileStorage fsr("cam_right.yml", FileStorage::READ);
  printf("Starting Calibration\n");
  cv::Mat K1, K2, R, F, E;
  cv::Vec3d T;
  cv::Mat D1, D2;
  fsl["K1"] >> K1;
  fsr["K1"] >> K2;
  fsl["D1"] >> D1;
  fsr["D1"] >> D2;
  int flag = 0;
  flag |= CV_CALIB_FIX_INTRINSIC;
  
  cout << "Read intrinsics" << endl;
  
  stereoCalibrate(object_points, left_img_points, right_img_points, K1, D1, K2, D2, spl1.size(), R, T, E, F);

  cv::FileStorage fs1("cam_stereo.yml", cv::FileStorage::WRITE);
  fs1 << "K1" << K1;
  fs1 << "K2" << K2;
  fs1 << "D1" << D1;
  fs1 << "D2" << D2;
  fs1 << "R" << R;
  fs1 << "T" << T;
  fs1 << "E" << E;
  fs1 << "F" << F;
  
  printf("Done Calibration\n");

  printf("Starting Rectification\n");

  cv::Mat R1, R2, P1, P2, Q;
  stereoRectify(K1, D1, K2, D2, spl1.size(), R, T, R1, R2, P1, P2, Q);

  fs1 << "R1" << R1;
  fs1 << "R2" << R2;
  fs1 << "P1" << P1;
  fs1 << "P2" << P2;
  fs1 << "Q" << Q;

  printf("Done Rectification\n");

  cv::Mat lmapx, lmapy, rmapx, rmapy;
  Mat imgU1, imgU2;
  initUndistortRectifyMap(K1, D1, R1, P1, spl1.size(), CV_32F, lmapx, lmapy);
  initUndistortRectifyMap(K2, D2, R2, P2, spl1.size(), CV_32F, rmapx, rmapy);

  cv::remap(spl1, imgU1, lmapx, lmapy, cv::INTER_LINEAR);
  cv::remap(spl2, imgU2, rmapx, rmapy, cv::INTER_LINEAR);

  imshow("image1", imgU1);
  imshow("image2", imgU2);

  waitKey(0);

  return 0;
}
