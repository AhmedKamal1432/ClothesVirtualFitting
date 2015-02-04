#include "flood_fill.h"
#include "help.h"
#include "face_detection.h"
#include "plot_tshirt.h"

#define shirt_hight_start 50

int main(int argc, char** argv)
{
  
  //argv[1] main image
  //argv[2] teshert

  Mat main_img , src_image , body , shirt;
  src_image = imread(argv[1]);
  main_img = resize_body(src_image);
  body = main_img.clone();
  imshow("body" , main_img);

  Mat edge = call_canny(80,body);

  int  mid_width_body = mid_body(edge  , "body mid");
  // imshow("mid" , mid);

  src_image = imread(argv[2]);
  main_img = resize_tshirt(src_image);
  shirt = main_img.clone(); 
  imshow("shirt" , main_img);

  edge = call_canny(80,shirt);

  int mid_width_shirt = mid_body(edge ,"shirt mid");
  // imshow("shirt mid" , mid);
  printf("body_width_mid = %d,    Tshirt _width_mid =%d \n", mid_width_body , mid_width_shirt);

  Rect face = call_face_detect(body);
  printf("x =%d y =%d  w =%d h =%d \n",face.x ,face.y,face.height,face.width );
  // Point pt1(face.x, face.y); // Display detected faces on main window - live stream from camera
  // Point pt2((face.x + face.height), (face.y + face.width));
  // rectangle(body, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
  // imshow ("face", body);
  
  printf("ggg\n");
  Mat im_gray;
  cvtColor(shirt,im_gray,CV_RGB2GRAY);

  imshow("shirt gray" , im_gray);
  
  Mat shirt_bw = im_gray > 180;
  imshow(" shirt binary" , shirt_bw);
  printf("mid %d %d\n", mid_width_body,mid_width_shirt);
  int start_heigth_body = face.y+face.height -shirt_hight_start, start_heigth_shirt = shirt_hight_start;

  Mat ans =  plot_tshirt(mid_width_body,mid_width_shirt,start_heigth_body, start_heigth_shirt,body ,shirt, shirt_bw );

  waitKey(0);
}

