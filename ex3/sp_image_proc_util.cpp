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
#include "SPBPriorityQueue.h"
}

#include "sp_image_proc_util.h"

#define N 3

using namespace cv;

//helpermethod
double* colToArray(Mat* m) {//global - notice the names is unintuative at first
  double* array = (double*)malloc(m->rows * sizeof(double));
  for (int i = 0; i < m->rows; i++) {
    array[i] = m->at<float>(i,0);
  }
  return array;
}
double* rowToArray(Mat* m) {//local
  double* array = (double*)malloc(m->cols * sizeof(double));
  for (int i = 0; i < m->cols; i++) {
    array[i] = m->at<float>(0,i);
  }

  return array;
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
  Mat rgb_hists;

  /// Compute the histograms: 
  /// The results will be store in brg_hists.
  /// The output type of the matrices is CV_32F (float)
  double* data;
  for (int i = 0; i < N; i++) {
    //instead of bgr(like the opencv example we use rgb for negeting cognitive dissonance 
    calcHist(&bgr_planes[N - i - 1], nImages, 0, Mat(), rgb_hists, 1, &nBins, &histRange);
    data =  colToArray(&rgb_hists);
    rgbHist[i] = spPointCreate(data, nBins, imageIndex);
    free(data);
  }
  
  return rgbHist;
}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB) {
  if (rgbHistA == NULL || rgbHistB == NULL)
    return -1;
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
    data = rowToArray(&m1);
    pointArray[i] = spPointCreate(data, ds1.cols, imageIndex);
    free(data);

  }

  *nFeatures = ds1.rows;
  detect.release();

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
      spBPQueueEnqueue(priorityQueue, i, spPointL2SquaredDistance(databaseFeatures[i][j], queryFeature));
    }
  }

  // Extract the results to the result array.
  for (int i = 0; i < kClosest; i++) {
    spBPQueuePeek(priorityQueue, peekElement);
    indexResult[i] = peekElement->index;
    spBPQueueDequeue(priorityQueue);
    //printf("rank %d:  , image index: %d,distace: %hf\n",i+1,peekElement->index,peekElement->value);
  }

  // Clean internal memory.
  spBPQueueDestroy(priorityQueue);
  free(peekElement);
  return indexResult;
}
