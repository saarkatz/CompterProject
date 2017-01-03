#include <malloc.h>
#include <stdio.h>
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "sp_image_proc_util.h"

using namespace cv;

SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {
  return NULL;
}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {
  return NULL;
}

SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures) {
  return NULL;
}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
  SPPoint*** databaseFeatures, int numberOfImages,
  int* nFeaturesPerImage) {
  return NULL;
}

Mat* runFunc(const Mat* source, int k, void (*func)(Mat*, const Mat*, int, int, int)) {
  Mat* result = new Mat(source->rows - k + 1, source->cols - k + 1, source->type());
  for (int i = 0; i < result->rows; i++) {
    for (int j = 0; j < result->cols; j++) {
      func(result, source, k, i, j);
    }
  }
  return result;
}

void runFrame(Mat* result, const Mat* source, int k, double** frame, int i, int j) {
  for (int l = 0; l < k; l++) {
    for (int m = 0; m < k; m++) {
      result->at<uchar>(i, j) += frame[l][m] * source->at<uchar>(i + l - k / 2 + 1, j + m - k / 2 + 1);
    }
  }
}

int main() {
  Mat image;
  Mat* result;
  image = imread("images/img8.png", CV_LOAD_IMAGE_GRAYSCALE);
  if (image.empty()) {
    return -1;
  }

  //for (int i = 0; i < image.cols; i++) {
  //  for (int j = 0; j < image.rows; j++) {
  //    if ((int)std::sqrt(j*j + i*i) % 20 >= 10) {
  //      image.at<uchar>(j, i) *= 0.75;
  //    }
  //  }
  //}

  double** frame = new double*[3];
  for (size_t i = 0; i < 3; i++) {
    frame[i] = new double[3];
    for (size_t j = 0; j < 3; j++) {
      if (i == 0) frame[i][j] = -1 / 6.0;
      if (i == 0) frame[i][j] = 0;
      if (i == 2) frame[i][j] = 1 / 6.0;
    }
  }
  //frame[1][1] = 0.2;

  result = runframe(&image, 3, frame);

  cv::namedWindow("Original", WINDOW_AUTOSIZE);
  cv::namedWindow("Result", WINDOW_AUTOSIZE);
  cv::imshow("Original", image);
  cv::imshow("Result", *result);

  cv::waitKey(0);
  return 0;
}

