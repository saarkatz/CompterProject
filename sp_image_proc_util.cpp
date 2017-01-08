#include <malloc.h>

#include <opencv2/imgproc.hpp> // calcHist
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

extern "C" {
#include "SPPoint.h"
}

#include "sp_image_proc_util.h"

#define N 3

using namespace cv;

//helpermethod
double* matrixToArray(Mat* m) {
  double* array = (double*)malloc(m->rows * sizeof(double));
  for (int i = 0; i < m->rows; i++) {
    array[i] = m->at<float>(i);
  }
  return array;
}

Mat* pointToMatrix(SPPoint* point) {
  Mat* mat = new Mat(spPointGetDimension(point), 1, CV_32F);
  for (int i = 0; i < spPointGetDimension(point); i++) {
    mat->at<float>(i) =  spPointGetAxisCoor(point, i);
  }
  return mat;
}

SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {
  if (str == NULL || nBins <= 0)
    return NULL;
  SPPoint** rgbHist = (SPPoint**)malloc(N * sizeof(SPPoint*));
  if (rgbHist == NULL)
    return NULL;

  // Read the image from the file.
  Mat image = imread(str, CV_LOAD_IMAGE_COLOR);

  if (image.empty()) {
    return NULL;
  }

  float range[] = { 0, 256 };
  const float* histRange = { range };
  int nImages = 1;

  // Separate the image in 3 places ( B, G and R )
  std::vector<Mat> bgr_planes;
  split(image, bgr_planes);

  //Output
  Mat rgb_hists[N];

  /// Compute the histograms: 
  /// The results will be store in brg_hists.
  /// The output type of the matrices is CV_32F (float)
  for (int i = 0; i < N; i++) {
    //instead of bgr(like the opencv example we use rgb for negeting cognitive dissonance 
    calcHist(&bgr_planes[N - i - 1], nImages, 0, Mat(), rgb_hists[i], 1, &nBins, &histRange);
    //rgb_hists[i].convertTo(rgb_hists[i], CV_64F);
    rgbHist[i] = spPointCreate(matrixToArray(&rgb_hists[i]), nBins, imageIndex);
  }
  return rgbHist;
}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {
  if (rgbHistA == NULL || rgbHistB == NULL)
    return -1;
  int dim = spPointGetDimension(rgbHistA[0]);
  double sum = 0;
  for (int i = 0; i<N; i++) {
    sum += 0.33*spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
  }
  return sum;
}
SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures) {
  return NULL;
}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
  SPPoint*** databaseFeatures, int numberOfImages,
  int* nFeaturesPerImage) {
  return NULL;
}

int drawRGBHist(SPPoint** rgb_hists, int nBins) {
  //*****************************************************************
  // This is not relevant for the Assignment - You can read it for fun
  //*****************************************************************
  // Draw the histograms for B, G and R
  int hist_w = 512;
  int hist_h = 400;
  int bin_w = cvRound((double)hist_w / nBins);

  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

  Mat* hists[N];
  Scalar scalars[N];
  scalars[0] = Scalar(255, 0, 0);
  scalars[1] = Scalar(0, 255, 0);
  scalars[2] = Scalar(0, 0, 255);

  for (int i = 0; i < N; i++) {
    hists[i] = pointToMatrix(rgb_hists[N - i - 1]);
  }

  /// Normalize the result to [ 0, histImage.rows ]
  for (int i = 0; i < N; i++) {
    normalize(*hists[i], *hists[i], 0, histImage.rows, NORM_MINMAX, -1, Mat());
  }

  /// Draw for each channel
  for (int i = 1; i < nBins; i++) {
    for (int j = 0; j < N; j++) {
      line(histImage,
        Point(bin_w * (i - 1), hist_h - cvRound(hists[j]->at<float>(i - 1))),
        Point(bin_w * (i), hist_h - cvRound(hists[j]->at<float>(i))),
        scalars[j], 2, 8, 0);
    }
  }

  /// Display
  namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
  imshow("calcHist Demo", histImage);

  waitKey(0);

  return 0;
}

int main() {
  int nBins = 256;
  SPPoint** histograms = spGetRGBHist("images/img12.png", 1, nBins);
  if (histograms == NULL) {
    return -1;
  }
  drawRGBHist(histograms, nBins);
  return 0;
}