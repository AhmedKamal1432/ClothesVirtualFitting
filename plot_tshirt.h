Mat plot_tshirt(int mid_width_body,int mid_width_shirt,
                int start_heigth_body,int start_heigth_shirt,
                Mat body , Mat shirt,
                Mat shirt_bw ){

  printf("start of ploting\n");

  Size s = shirt.size();
  int sh = s.height;  int sw = s.width;

  Mat ans = body.clone();
  s = body.size();
  int bh = s.height,bw = s.width;
  //right side
 int h_it_s = start_heigth_shirt;
 int h_it_b =start_heigth_body;

 printf("second of ploting\n");

printf(" mid  width in two %d %d ",mid_width_body , mid_width_shirt);
 //  while(h_it_s < sh && shirt_bw.at<uchar>(h_it_s,mid_width_shirt) < 100){
 //    printf("in\n");
 //    int w_it_s = mid_width_shirt;
 //    int w_it_b = mid_width_body;

 //    while(w_it_s < sw && shirt_bw.at<uchar>(h_it_s,w_it_s) < 100){
 //      ans.at<uchar>(h_it_b,w_it_b) = shirt.at<uchar>(h_it_s,w_it_s);
 //      w_it_s++;
 //      w_it_b++;
 //    }

 //   h_it_b++; h_it_s++;
 // }

REP(i,bh){
  REP(j,bw/2){
    if(i < sh && j < sw/2){
      if(shirt_bw.at<uchar> (i,mid_width_shirt + j) < 50)
        ans.at<Vec3b>(i+start_heigth_body,mid_width_body + j) = shirt.at<Vec3b>(i,mid_width_shirt + j);
    }
  }
}

 imshow("right part" , ans);

h_it_s = start_heigth_shirt;
h_it_b =start_heigth_body;

 //  while(h_it_s < sh && shirt_bw.at<uchar>(h_it_s,mid_width_shirt) < 100){
 //    int w_it_s = mid_width_shirt;
 //    int w_it_b = mid_width_body;

 //    while(w_it_s > 0 && shirt_bw.at<uchar>(h_it_s,w_it_s) < 100){
 //      ans.at<uchar>(h_it_b,w_it_b) = shirt.at<uchar>(h_it_s,w_it_s);
 //      w_it_s--;
 //      w_it_b--;
 //    }

 //   h_it_b++; h_it_s++;
 // }
 printf("end of ploting\n");

REP(i,bh){
  REP(j,bw/2){
    if(i < sh && j < sw/2){
      if(shirt_bw.at<uchar> (i,mid_width_shirt - j) < 50)
        ans.at<Vec3b>(i+start_heigth_body,mid_width_body - j) = shirt.at<Vec3b>(i,mid_width_shirt - j);
    }
  }
}

 imshow("left part" , ans);
 // imwrite("left part" , ans);
 imwrite("images/right part.jpg" , ans);
 return ans;
}