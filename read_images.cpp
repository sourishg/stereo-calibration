#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int x = 0;

int main(int argc, char const *argv[])
{
  VideoCapture cap1(0);
  VideoCapture cap2(1);
  Mat img1, img_res1, img2, img_res2;
  while (1) {
    cap1 >> img1;
    cap2 >> img2;
    resize(img1, img_res1, Size(480, 270));
    resize(img2, img_res2, Size(480, 270));
    //cout << frame.rows << " " << frame.cols << endl;
    imshow("IMG1", img_res1);
    imshow("IMG2", img_res2);
    if (waitKey(30) > 0) {
      x++;
      char filename1[200], filename2[200];
      sprintf(filename1, "left%d.png", x);
      sprintf(filename2, "right%d.png", x);
      cout << "Saving img pair " << x << endl;
      imwrite(filename1, img_res1);
      imwrite(filename2, img_res2);
    }
  }
  return 0;
}
