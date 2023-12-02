#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
  cv::Mat image, erosao, dilatacao, abertura, fechamento, abertfecha,fechaabert;
  cv::Mat str;

  if (argc != 2) {
    std::cout << "morfologia entrada saida\n";
  }

  image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

//  image = cv::imread(argv[1], -1);
  
  if(image.empty()) {
    std::cout << "Erro ao carregar a imagem: " << argv[1] << std::endl;
    return -1;
  }

  cv::bitwise_not(image,image);

  // elemento estruturante
  str = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1,11));
  
  // fechamento
  cv::morphologyEx(image, fechamento, cv::MORPH_CLOSE, str);

  cv::Mat matArray[] = {fechamento};
  cv::hconcat(matArray, 1, image);

  cv::bitwise_not(image,image);

  cv::imshow("morfologia", image);

  cv::waitKey();
  return 0;
}
