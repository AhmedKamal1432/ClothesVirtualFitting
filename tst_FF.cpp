#include "fill_in_edges_white.h"
#include "canny.h"

int main(int argc, char const *argv[])
{
  Mat _src , src , edged , fill ,ss;
  Mat tmp2 = imread(argv[1],1);
  ss.create( tmp2.size(), tmp2.type() );

  resize(tmp2 ,_src,_src.size(),0.4,0.4,INTER_LINEAR);
  medianBlur(_src, ss, 1);
  edged =  call_canny(80 , ss );
  imshow("edges" , edged);
  // print_mat(edged);
    Mat  ed_d = canny_digtal(edged);
    // Mat ed_d = edge_thickness(edged);
    fill = fill_in_edges_white(edged , 225); 
    imshow("filed" , fill);
  waitKey(0);
  return 0;
}
