#define REP(i, n)   for(int i=0;i<(int)(n);++i)
#define ras_th 20
#define LOOP(i,b,n)   for(int i=(b);i<(int)(n);++i)

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
typedef unsigned char    uchart;

#include <stdio.h>
#include <string.h>
#include <ctype.h>


using namespace cv;
using namespace std;

// floodFill(InputOutputArray image, Point seedPoint, Scalar newVal, Rect* rect=0, Scalar loDiff=Scalar(), Scalar upDiff=Scalar(), int flags=4 )


Mat fill_in_edges_white (Mat src , int color){
Size s = src.size();
int n = s.height ,  m = s.width;
int count = 0;
bool found , ras = false,left = false , right = true;;
bool start = false;
bool end = false;
  printf("n =%d m =%d \n",n,m );
 
  LOOP(i,5,n){
    if(ras)
      count++;
    LOOP(j,5,m){
        // printf(" i = %d  j = %d\n", i,j);
       if(src.at<uchart>(i,j) > 20 ){
        // printf("ras and left is true\n");
        // printf("left == true i = %d  j = %d\n", i,j);
         // begin is appear;
         ras = true;
         // left is up
         left = true;

        }
      if(count > ras_th & left){
        printf(" i = %d  j = %d\n", i,j);
        floodFill(src, Point(i,j), color );
        end = true;
        break;
      }
    }
    left = false;
    if(end)
      break ;
  }
  return src;
}


Mat edge_thickness(Mat src){
  Mat ans;
  ans.create( src.size(), src.type() );
  Size s = src.size();
  int n = s.height ,  m = s.width;

  REP(i,n){
    REP(j,m)
      printf("%d ",src.at<uchart>(i,j) );
    printf("\n");
  }

  LOOP(i,5,n-5){
    LOOP(j,5,m-5){
      if(src.at<uchart>(i,j) > 120){
        ans.at<uchart>(i,j) = src.at<uchart>(i,j);
        // ans.at<uchart>(i-1,j) = src.at<uchart>(i,j);
        // ans.at<uchart>(i+1,j) = src.at<uchart>(i,j);
        // ans.at<uchart>(i,j-1) = src.at<uchart>(i,j);
        ans.at<uchart>(i-1,j-1) = src.at<uchart>(i,j);
        ans.at<uchart>(i+1,j-1) = src.at<uchart>(i,j);
        // ans.at<uchart>(i,j+1) = src.at<uchart>(i,j);
        ans.at<uchart>(i-1,j+1) = src.at<uchart>(i,j);
        ans.at<uchart>(i+1,j+1) = src.at<uchart>(i,j);
      }
    }
  }
  imshow("edge_thickness" , ans);
  return ans;
}

void print_mat(Mat src){
  Size s = src.size();
  int n = s.height ,  m = s.width;

  REP(i,n){
    REP(j,m)
      printf("%d ",(int)src.at<uchart>(i,j) );
    printf("\n");
  }
}