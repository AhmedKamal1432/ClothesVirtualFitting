
#define BODY_X 670.0
#define BODY_Y 570.0

#define TSHIRT_X 400.0
#define TSHIRT_Y 370.0

void print_mat(Mat src){
  Size s = src.size();
 int n = s.height, m = s.width;
  REP(i,n)
    REP(j,m)
      printf("%d ", src.at<uchart>(i,j));
}

Mat resize_body(Mat src_image){

  Size s = src_image.size();

  double fy = BODY_Y / (double)s.width;
  double fx = BODY_X / (double)s.height;

  Mat main_img;
  resize(src_image ,main_img,main_img.size(),fy,fx,INTER_LINEAR);

  return main_img;
}

Mat resize_tshirt(Mat src_image){

  Size s = src_image.size();

  double fy = TSHIRT_Y / (double)s.width;
  double fx = TSHIRT_X / (double)s.height;

  Mat main_img;
  resize(src_image ,main_img,main_img.size(),fy,fx,INTER_LINEAR);

  return main_img;
}

 // input binary image;

int mid_body(Mat src , char* pic_name){
  Size s = src.size();
  int n = s.width, m=s.height;
  printf(" n ,m %d %d\n",n,m );
  Mat im = src.clone();
  int mid = 0, tc = 0;
  REP(j,m){
    int cnt = 0 , sum =0;
    REP(i,n){
      if(im.at<uchart>(j,i) > 0){
        cnt++; sum+=i;
      }
    }
    if(cnt > 0){
      int b = (int) sum/cnt;
      mid+=b;
      tc++;
    }
  }

  mid /= tc;
  REP(j,m){
    im.at<uchart>(j,mid) = 250;
  }

  imshow(pic_name, im);
  return mid;
} 
