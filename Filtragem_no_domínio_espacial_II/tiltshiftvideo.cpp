#include <iostream>
#include <cstdio>
#include <cmath>
#include <opencv2/opencv.hpp>

int main(int argvc, char** argv){
  cv::VideoCapture cap;
  cv::Mat frame, result, blur, lixo, frame2, blur2;
  float l1, l2, d = 50.0, tmp;
  int width, height;

  cap.open("WhatsApp Video 2023-09-24 at 14.59.05.mp4");

  if(!cap.isOpened()){
    return -1;
  }

  cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);  
  width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
  height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);

  l1 = height/2.5; 
  l2 = height-l1;

  cv::VideoWriter video("tiltshift.avi", cv::VideoWriter::fourcc('M','J','P','G'),30,cv::Size(width,height),true);
  if(!video.isOpened()){
    return -1;
  }

  while(1){
    cap >> frame;
    cv::Mat pOrig(frame.size(),CV_32F);
    cv::Mat pBlur(frame.size(),CV_32F);

    cv::GaussianBlur(frame,blur,cv::Size(9,9),0,0);

    for(int x = 0; x< pOrig.rows; x++){
      tmp = 1/2.0 * (tanh((x-l1)/d) - tanh((x-l2)/d));
      for(int y = 0; y< pOrig.cols;y++){
        pOrig.at<float>(x,y) = tmp;
        pBlur.at<float>(x,y) = (1-tmp);
      }
    }
    cv::Mat cannel_orig[3], cannel_blur[3];
    cv::split(frame, cannel_orig);
    cv::split(blur, cannel_blur);

    for(int i=0; i<3; i++){
      cv::multiply(cannel_orig[i],pOrig,cannel_orig[i],1,CV_8UC3);
      cv::multiply(cannel_blur[i],pBlur,cannel_blur[i],1,CV_8UC3);
    }

    merge(cannel_orig,3,frame2);
    merge(cannel_blur,3,blur2);

    result = frame2+blur2;

    video.write(result);
    cap >> lixo >> lixo;
  }

  return 0;
}
