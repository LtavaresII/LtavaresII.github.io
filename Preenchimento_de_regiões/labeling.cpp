#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv) {
  cv::Mat image, realce;
  int width, height;
  int nohole, hole;

  cv::Point p,b;
  image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);

  if (!image.data) {
    std::cout << "imagem nao carregou corretamente\n";
    return (-1);
  }

  width = image.cols;
  height = image.rows;
  std::cout << width << "x" << height << std::endl;

  p.x = b.x = 0;
  p.y = b.y = 0;

  // retira os objetos das bordas
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if(i == 0 || j == 0 || i == height-1 || j == width-1){
          p.x = j;
          p.y = i;

          cv::floodFill(image, p, 0);
          cv::floodFill(image, p, 1);
      }
    }
  }

  // busca objetos presentes
  hole = 0;
  nohole = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (image.at<uchar>(i, j) == 0) {
        // achou um objeto
        hole++;
        // para o floodfill as coordenadas
        // x e y são trocadas.
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        cv::floodFill(image, p, 1);
      }

      if(image.at<uchar>(i, j) == 255){
        // achou um objeto
        nohole++;
        // para o floodfill as coordenadas
        // x e y são trocadas.
        p.x = j;
        p.y = i;
        // preenche o objeto com o contador
        cv::floodFill(image, p, 254);
      }
    }
  }
  std::cout << "a figura tem " << hole << " regiões com buracos\n";
  std::cout << "a figura tem " << nohole-hole << " regiões sem buracos\n";
  cv::imshow("image", image);
  cv::imwrite("labeling.png", image);
  cv::waitKey();
  return 0;
}
