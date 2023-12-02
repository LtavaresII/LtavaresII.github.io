#include <iostream>
#include <vector>
#include <stdlib.h>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define RADIUS 20

void swapQuadrants(cv::Mat& image) {
  cv::Mat tmp, A, B, C, D;

  // se a imagem tiver tamanho impar, recorta a regiao para o maior
  // tamanho par possivel (-2 = 1111...1110)
  image = image(cv::Rect(0, 0, image.cols & -2, image.rows & -2));

  int centerX = image.cols / 2;
  int centerY = image.rows / 2;

  // rearranja os quadrantes da transformada de Fourier de forma que 
  // a origem fique no centro da imagem
  // A B   ->  D C
  // C D       B A
  A = image(cv::Rect(0, 0, centerX, centerY));
  B = image(cv::Rect(centerX, 0, centerX, centerY));
  C = image(cv::Rect(0, centerY, centerX, centerY));
  D = image(cv::Rect(centerX, centerY, centerX, centerY));

  // swap quadrants (Top-Left with Bottom-Right)
  A.copyTo(tmp);
  D.copyTo(A);
  tmp.copyTo(D);

  // swap quadrant (Top-Right with Bottom-Left)
  C.copyTo(tmp);
  B.copyTo(C);
  tmp.copyTo(B);
}

int main(int argc, char** argv) {
  cv::Mat image, padded, complexImage, imaginaryInput, filter, imagegray, tmp;
  cv::Mat_<float> realInput, zeros;
  std::vector<cv::Mat> planos; 

  image = imread(argv[1], cv::IMREAD_GRAYSCALE);
  if (image.empty()) {
    std::cout << "Erro abrindo imagem" << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // expande a imagem de entrada para o melhor tamanho no qual a DFT pode ser
  // executada, preenchendo com zeros a lateral inferior direita.
  int dft_M = cv::getOptimalDFTSize(image.rows);
  int dft_N = cv::getOptimalDFTSize(image.cols); 

  cv::copyMakeBorder(image, padded, 0, dft_M - image.rows, 0, dft_N - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

  // parte imaginaria com valores nulos
  zeros = cv::Mat_<float>::zeros(padded.size());

  // prepara a matriz complexa
  complexImage = cv::Mat(padded.size(), CV_32FC2, cv::Scalar(0));

  // função de transferencia
  filter = complexImage.clone();

  // matriz temporaria
  tmp = cv::Mat(dft_M,dft_N,CV_32F);

  // filtro homomorfico
  float gamah = 55,gamal = 40,c = 70,D0 = 35;
  int M = dft_M;
  int N = dft_N;

  for (int i = 0; i < dft_M; i++) {
    for (int j = 0; j < dft_N; j++) {
      tmp.at<float>(i, j) = (gamah - gamal)*(1.0 - exp(-1.0*(float)c*((((float)i - M / 2.0)*((float)i - M / 2.0) + ((float)j - N / 2.0)*((float)j - N / 2.0)) / (D0*D0)))) + gamal;
    }
  }

  cv::Mat comps[] = {tmp,tmp};
  cv::merge(comps, 2, filter);

  // cria a componente real
  realInput = cv::Mat_<float>(padded);

  // insere as duas componentes no array de matrizes
  planos.push_back(realInput);
  planos.push_back(zeros);

  // combina o array de matrizes
  merge(planos, complexImage);

  // calcula a DFT
  cv::dft(complexImage, complexImage);

  // troca os quadrantes
  swapQuadrants(complexImage);

  // aplica o filtro frequencial
  mulSpectrums(complexImage,filter,complexImage,0);

  // limpa o array de planos
  planos.clear();
  
  // separa as partes reais e imaginarias
  split(complexImage, planos);

  // recompoe os planos
  merge(planos,complexImage);

  // troca novamente os quadrantes
  swapQuadrants(complexImage);

  // calcula a dft inversa
  idft(complexImage,complexImage);

  // limpa o array de planos
  planos.clear();

  // separa as partes reais e imaginaria
  split(complexImage,planos);

  // normaliza a parte real para exibicao
  cv::normalize(planos[0], planos[0], 0, 1, cv::NORM_MINMAX);

  cv::imshow("filtrada", planos[0]);
  cv::imshow("original", image);
  cv::imwrite("dft-filter.png", image);

  cv::waitKey();
  return EXIT_SUCCESS;
}
