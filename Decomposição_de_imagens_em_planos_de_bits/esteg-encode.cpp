#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char**argv) {
  cv::Mat imagemPortadora;
  cv::Vec3b valPortadora, valEscondida;
  int nbits = 3;

  imagemPortadora = cv::imread(argv[1], cv::IMREAD_COLOR);
  
  if (imagemPortadora.empty()) {
    std::cout << "imagem nao carregou corretamente" << std::endl;
    return (-1);
  }

  for (int i = 0; i < imagemPortadora.rows; i++) {
    for (int j = 0; j < imagemPortadora.cols; j++) {
      valPortadora = imagemPortadora.at<cv::Vec3b>(i, j);
      valPortadora[0] = valPortadora[0] << (8-nbits);
      valPortadora[1] = valPortadora[1] << (8-nbits);
      valPortadora[2] = valPortadora[2] << (8-nbits);
      valEscondida[0] = valPortadora[0];
      valEscondida[1] = valPortadora[1];
      valEscondida[2] = valPortadora[2];
      imagemPortadora.at<cv::Vec3b>(i, j) = valEscondida;
    }
  }
  imwrite("esteganografia.png", imagemPortadora);
  return 0;
}
