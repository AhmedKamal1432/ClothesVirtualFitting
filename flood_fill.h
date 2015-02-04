#include "canny.h"


#define SCALE_X 0.4 
#define SCALE_Y 0.4 

// take an RGB image  of body or TShirt and return a binsry image of filled
Mat call_floodFill(Mat src_image ,Point start_point = Point(0,0) ,int  color = 255 )
{
   printf("px %d  py%d\n", start_point.x ,start_point.y);
  imshow("src",src_image);
  int morph_elem = 2;
  int morph_size = 10;

  Mat _src , src , edged , fill ,ss;
  ss.create( src_image.size(), src_image.type() );

  resize(src_image ,_src,_src.size(),SCALE_X,SCALE_Y,INTER_LINEAR);

  Size s = _src.size();
  int n = s.height, m = s.width;

  // if(start_point.x == 0 && start_point.y == 0){
  //   start_point = Point(m/2,n/2);
  //   printf("m = %d n =%d \n",m , n );     
  // }

  medianBlur(_src, ss, 1);
  edged =  call_canny(70 , ss );
   

  // to binary
  ///////////////
  Mat im_gray;
  cvtColor(edged,im_gray,CV_RGB2GRAY);
  Mat img_bw = im_gray > 40;
  imshow("binary" , img_bw);
 /////////////////////////////////////////////
 
  Mat mask = img_bw.clone();
  copyMakeBorder(mask, mask, 1, 1, 1, 1, cv::BORDER_REPLICATE);
  
  imshow("mask" , mask);
  /////////////////

  //Closed 
   Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

  Mat closed;
  morphologyEx( img_bw, closed, MORPH_CLOSE, element );

  imshow("closed" , closed);

/////////////////////////////////////////
  // fill = fill_in_edges_white(closed , 255  ); 

  fill = closed.clone();
  floodFill(closed, Point(0,0), 255 );
  imshow("filed" , fill); 

///////////////////////////////////////
return fill;
}
