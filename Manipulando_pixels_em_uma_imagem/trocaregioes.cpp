#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**){
  cv::Mat image,Q1,Q2,Q3,Q4;
  int x0,y0,width,height,x,y;

  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
  
  x0 = 0;
  y0 = 0;
  width = image.cols;
  height = image.rows;
  x = height/2;
  y = width/2;
  
  Q1 = image(cv::Range(x0,x), cv::Range(y0,y));
  Q2 = image(cv::Range(x0,x),cv::Range(y,width));
  Q3 = image(cv::Range(x,height),cv::Range(y0,y));
  Q4 = image(cv::Range(x,width),cv::Range(y,height));
  cv::Mat image2 = cv::Mat::zeros(image.size(),CV_8UC1);

  Q1.copyTo(image2(cv::Range(x,width),cv::Range(y,height)));
  Q2.copyTo(image2(cv::Range(x,height),cv::Range(y0,y)));
  Q3.copyTo(image2(cv::Range(x0,x),cv::Range(y,width)));
  Q4.copyTo(image2(cv::Range(x0,x), cv::Range(y0,y)));
  
  cv::imshow("janela", image2);
  cv::imwrite("troca.png",image2); 
  cv::waitKey(0);

  return 0;
}
