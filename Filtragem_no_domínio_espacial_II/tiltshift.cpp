#include <iostream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>

double alfa;
int alfa_slider = 0;
int alfa_slider_max = 100;

int top_slider = 0;
int top_slider_max = 100;

int center_slider = 0;
int center_slider_max = 100;

cv::Mat image1, image2, blended;
cv::Mat imageTop; 

char TrackbarName[50];

void on_trackbar_blend(int, void*){
 alfa = (double) alfa_slider/alfa_slider_max ;
 cv::addWeighted(image2, 1-alfa, imageTop, alfa, 0.0, blended);
 cv::imshow("Tilt Shift", blended);
}

void on_trackbar_line(int, void*){
  image2.copyTo(imageTop);

  int width = image2.size().width;
  int height = image2.size().height;
  int limit = top_slider*height/100;
  int base = center_slider*height/100;

  if(limit > 0){
    if(base >=0 && base <= height-limit){
      cv::Mat tmp = image1(cv::Rect(0, base, width, limit));
      tmp.copyTo(imageTop(cv::Rect(0, base, width, limit)));
    }else{
      cv::Mat tmp = image1(cv::Rect(0, 0, width, limit));
      tmp.copyTo(imageTop(cv::Rect(0, 0, width, limit)));
    }
  }
  on_trackbar_blend(alfa_slider,0);
}

int main(int argvc, char** argv){
  image1 = cv::imread("blend2.jpg");
  resize(image1,image1,cv::Size(640,480));
  image1.copyTo(image2);
  cv::namedWindow("Tilt Shift", 1);
  image2.convertTo(image2, CV_32F);

  float median[] = {1,2,1,
                    2,4,2,
                    1,2,1};

  cv::Mat mask = cv::Mat(3,3,CV_32F,median);
  scaleAdd(mask,1/16.0, cv::Mat::zeros(3,3,CV_32F),mask);

  for(int i=0; i<8; i++){
    cv::filter2D(image2,image2,image2.depth(),mask,cv::Point(1,1),0);
  }

  image2.convertTo(image2, CV_8U);
  image2.copyTo(imageTop);

  std::sprintf( TrackbarName, "Decay x %d", alfa_slider_max );
  cv::createTrackbar( TrackbarName, "Tilt Shift",
                      &alfa_slider,
                      alfa_slider_max,
                      on_trackbar_blend );
  on_trackbar_blend(alfa_slider, 0 );
  
  std::sprintf( TrackbarName, "Height x %d", top_slider_max );
  cv::createTrackbar( TrackbarName, "Tilt Shift",
                      &top_slider,
                      top_slider_max,
                      on_trackbar_line );
  on_trackbar_line(top_slider, 0 );

  std::sprintf( TrackbarName, "Center x %d", top_slider_max );
  cv::createTrackbar( TrackbarName, "Tilt Shift",
                      &center_slider,
                      center_slider_max,
                      on_trackbar_line );
  on_trackbar_line(center_slider, 0 );

  cv::waitKey(0);
  cv::imwrite("tiltshift.jpg", blended);
  return 0;
}
