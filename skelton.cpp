#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

using namespace std;
using namespace cv;

int main(int argc, char** argv){

  Mat img_tani = imread(argv[1],1);
    Mat im_gray;
  cvtColor(img_tani,im_gray,CV_RGB2GRAY);
  imshow("gray",im_gray);
  Mat img_temp = im_gray > 160;
  Mat img;
  bitwise_not ( img_temp, img );
  imshow("binary" , img);
  
  cv::threshold(img, img, 127, 255,THRESH_BINARY); 
  cv::Mat skel(img.size(), CV_8UC1, cv::Scalar(0));
  cv::Mat temp;
  cv::Mat eroded;
  cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3, 3));
  bool done;
  // int i = 0;    
  do{
      cv::erode(img, eroded, element);
      cv::dilate(eroded, temp, element); // temp = open(img)
      cv::subtract(img, temp, temp);
      cv::bitwise_or(skel, temp, skel);
      eroded.copyTo(img);
      done = (cv::countNonZero(img) == 0);
    } while (!done);

// do
// {
//   cv::morphologyEx(img, temp, cv::MORPH_OPEN, element);
//   cv::bitwise_not(temp, temp);
//   cv::bitwise_and(img, temp, temp);
//   cv::bitwise_or(skel, temp, skel);
//   cv::erode(img, img, element);
 
//   double max;
//   cv::minMaxLoc(img, 0, &max);
//   done = (max == 0);
// } while (!done);

  cv::imshow("Skeleton", skel);
  cv::waitKey(0);
  return 0;
}