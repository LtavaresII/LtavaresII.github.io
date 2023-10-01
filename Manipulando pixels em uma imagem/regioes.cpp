#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**){
  cv::Mat image;
  int P1[2], P2[2];

  image= cv::imread("biel.png",cv::IMREAD_GRAYSCALE);
  if(!image.data)
    std::cout << "nao abriu biel.png" << std::endl;

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);

  std::cout << "Informe os pontos da região que ficarar negativa:" << std::endl;
  std::cout << "P1: " << std::endl;
  std::cin >> P1[0];
  std::cin >> P1[1];
  std::cout << "P2: " << std::endl;
  std::cin >> P2[0];
  std::cin >> P2[1];

  for(int i=P1[0];i<P1[1];i++){
    for(int j=P2[0];j<P2[1];j++){
      image.at<uchar>(i,j)= 255 - image.at<uchar>(i,j);
    }
  }

  cv::namedWindow("janela", cv::WINDOW_AUTOSIZE);
  cv::imshow("janela", image); 
  cv::imwrite("negativo.png",image); 
  cv::waitKey();

  return 0;
}
