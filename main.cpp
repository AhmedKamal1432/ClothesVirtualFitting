#include "flood_fill.h"
#include "help.h"

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

  int  body_width_mid = mid_body(edge , "body mid");
  // imshow("mid" , mid);

  src_image = imread(argv[2]);
  main_img = resize_tshirt(src_image);
  shirt = main_img.clone(); 
  imshow("shirt" , main_img);

  edge = call_canny(80,shirt);
  int shirt_width_mid = mid_body(edge ,"shirt mid");
  // imshow("shirt mid" , mid);
  printf("body_width_mid = %d,    Tshirt _width_mid =%d \n", body_width_mid , shirt_width_mid);

  waitKey(0);
}

