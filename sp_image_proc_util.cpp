#include <malloc.h>

#include <opencv2/imgproc.hpp> // calcHist
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui.hpp> //imshow, drawKeypoints, waitKey
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>//Mat
#include <opencv2/xfeatures2d.hpp>//SiftDescriptorExtractor
#include <opencv2/features2d.hpp>
#include <vector>

extern "C" {
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
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
    mat->at<float>(i) = spPointGetAxisCoor(point, i);
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
    printf("Image cannot be loaded - %s\n", str);
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
    rgbHist[i] = spPointCreate(matrixToArray(&rgb_hists[i]), nBins, imageIndex);
  }
  return rgbHist;
}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {
  if (rgbHistA == NULL || rgbHistB == NULL)
    return -1;
  int dim = spPointGetDimension(rgbHistA[0]);
  double sum = 0;
  for (int i = 0; i < N; i++) {
    sum += 0.33*spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
  }
  return sum;
}

SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures) {
  //working with one image
  //****moab code
  //Loading img - NOTE: Gray scale mode!
  cv::Mat src;
  src = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
  if (src.empty()) {
    return NULL;
  }
  //Key points will be stored in kp1;
  std::vector<cv::KeyPoint> kp1;
  //Feature values will be stored in ds1;
  cv::Mat ds1;
  //Creating  a Sift Descriptor extractor
  cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
    cv::xfeatures2d::SIFT::create(nFeaturesToExtract);
  //Extracting features
  //The features will be stored in ds1
  //The output type of ds1 is CV_32F (float)
  detect->detect(src, kp1, cv::Mat());
  detect->compute(src, kp1, ds1);


  //new code0
  SPPoint** pointArray = (SPPoint**)malloc(ds1.rows*sizeof(SPPoint*));
  double* data;
  for (int i = 0; i < ds1.rows; ++i) {
    //ds1.col(i) is the i-th column in Mat as a Mat
    Mat m1 = ds1.row(i);
    data = matrixToArray(&m1);
    pointArray[i] = spPointCreate(data, ds1.cols, imageIndex);
  }
  *nFeatures = ds1.rows;
  free(data);
  return pointArray;

}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
  SPPoint*** databaseFeatures, int numberOfImages,
  int* nFeaturesPerImage) {
  if (queryFeature == NULL || databaseFeatures == NULL ||
    nFeaturesPerImage == NULL || numberOfImages <= 1) {
    return NULL;
  }
  BPQueueElement* peekElement = (BPQueueElement*)malloc(sizeof(BPQueueElement));
  if (peekElement == NULL) {
    return NULL;
  }
  int* indexResult = (int*)malloc(kClosest * sizeof(int)); // Create result array
  if (indexResult == NULL) {
    return NULL;
  }

  SPBPQueue* priorityQueue = spBPQueueCreate(kClosest); // Create priority queue
  if (priorityQueue == NULL) {
    return NULL;
  }

  // For each image
  for (int i = 0; i < numberOfImages; i++) {
    // For each desctriptor in image
    for (int j = 0; j < nFeaturesPerImage[i]; j++)
    {
      // Add the L2 distance between query and the descriptor to the queue.
      spBPQueueEnqueue(priorityQueue, spPointGetIndex(databaseFeatures[i][j]),
        spPointL2SquaredDistance(databaseFeatures[i][j], queryFeature));
    }
  }

  // Extract the results to the result array.
  for (int i = 0; i < kClosest; i++) {
    spBPQueuePeek(priorityQueue, peekElement);
    indexResult[i] = peekElement->index;
  }

  // Clean internal memory.
  spBPQueueDestroy(priorityQueue);
  free(peekElement);
  return indexResult;
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

int maina() {
  int nBins = 256;
  SPPoint** histograms = spGetRGBHist("images/img12.png", 1, nBins);
  if (histograms == NULL) {
    return -1;
  }
  drawRGBHist(histograms, nBins);
/*
  SPPoint** histograms2 = spGetSiftDescriptors("images/img2.png", 1, nBins);
  SPPoint** histograms = spGetRGBHist("images/img12.png", 1, nBins);

  spRGBHistL2Distance()
  drawRGBHist(histograms2, nBins);
*/
  return 0;
}
