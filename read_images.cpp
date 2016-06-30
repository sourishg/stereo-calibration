#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include "popt_pp.h"

using namespace std;
using namespace cv;

int x = 0;

int main(int argc, char const *argv[])
{
  char* imgs_directory;
  char* extension;
  int im_width, im_height;

  static struct poptOption options[] = {
    { "img_width",'w',POPT_ARG_INT,&im_width,0,"Image width","NUM" },
    { "img_height",'h',POPT_ARG_INT,&im_height,0,"Image height","NUM" },
    { "imgs_directory",'d',POPT_ARG_STRING,&imgs_directory,0,"Directory to save images in","STR" },
    { "extension",'e',POPT_ARG_STRING,&extension,0,"Image extension","STR" },
    POPT_AUTOHELP
    { NULL, 0, 0, NULL, 0, NULL, NULL }
  };

  POpt popt(NULL, argc, argv, options, 0);
  int c;
  while((c = popt.getNextOpt()) >= 0) {}

  VideoCapture cap1(0);
  VideoCapture cap2(1);
  Mat img1, img_res1, img2, img_res2;
  while (1) {
    cap1 >> img1;
    cap2 >> img2;
    resize(img1, img_res1, Size(im_width, im_height));
    resize(img2, img_res2, Size(im_width, im_height));
    imshow("IMG1", img_res1);
    imshow("IMG2", img_res2);
    if (waitKey(30) > 0) {
      x++;
      char filename1[200], filename2[200];
      sprintf(filename1, "%sleft%d.%s", imgs_directory, x, extension);
      sprintf(filename2, "%sright%d.%s", imgs_directory, x, extension);
      cout << "Saving img pair " << x << endl;
      imwrite(filename1, img_res1);
      imwrite(filename2, img_res2);
    }
  }
  return 0;
}
