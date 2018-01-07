#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <bits/stdc++.h>

using namespace std;
using namespace cv;

std::vector<std::vector<cv::Point> > contours;

Mat src, src_gray;
int thresh = 90;

char* source_window = "Source image";
char* corners_window = "Corners detected";

void cornerHarris_demo(size_t largestContour);

int main(int agc,char **argv)
{
	src=imread(argv[1],0);
	Mat dest=src.clone(),destf=src.clone();
	Mat canny_img=src.clone();
	Canny(src,canny_img,15,80);
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(canny_img, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
        size_t largestContour = 0;
        size_t ndlargestContour = 0;

        for (size_t i = 0; i < contours.size(); i++)
         {
           if (cv::contourArea(contours[i]) > cv::contourArea(contours[largestContour]))
           {
              ndlargestContour=largestContour;
              largestContour = i;
            }

         }
        Mat  contour_img(src.rows,src.cols,CV_8UC1,Scalar(0));
        drawContours(contour_img,contours,largestContour,255,2);

       
    cornerHarris_demo(largestContour);

	return 0;
}






void cornerHarris_demo(size_t largestContour)
{

  Mat dst, dst_norm, dst_norm_scaled,duplicate=src;
  int blockSize = 2;
  int apertureSize = 3;
  double k = 0.04;

  /// Detecting corners
  cornerHarris( src, dst, blockSize, apertureSize, k, BORDER_DEFAULT );

  /// Normalizing
  normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
  convertScaleAbs( dst_norm, dst_norm_scaled );

  /// Drawing a circle around corners
  for( int j = 0; j < dst_norm.rows ; j++ )
     { for( int i = 0; i < dst_norm.cols; i++ )
        
          {
            if( (int) dst_norm.at<float>(j,i) > thresh)

            	if(pointPolygonTest(contours[largestContour], Point2f (i,j), true )>0)
              {
               circle(duplicate, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
              }
          }
     }

  /// Showing the result
  namedWindow( corners_window, CV_WINDOW_AUTOSIZE );
  imshow( corners_window,duplicate );
  waitKey(0);
}