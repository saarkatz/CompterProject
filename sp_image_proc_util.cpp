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
double* matrixToArray(Mat* m, int size) {
  double* array = (double*)malloc(size * sizeof(double));
  for (int i = 0; i < size; i++) {
    array[i] = m->at<float>(0, i);
  }
  return array;
}

/**
* Calculates the RGB channels histogram. The histogram will be stored in an array of
* of points, each point has the index imageIndex. The array has three entries,
* the first entry (i.e the array at index 0) us the red channel histogram, the second entery
* is the green channel histogram and the third entry is the blue channel histogram.
*
* @param str - The path of the image for which the histogram will be calculated
* @param nBins - The number of subdivision for the intensity histogram
* @param imageIndex - The index of the given image
* @return NULL if str is NULL or nBins <= 0 or allocation error occurred,
*  otherwise a two dimensional array representing the histogram.
*/
SPPoint** spGetRGBHist(const char* str, int imageIndex, int nBins) {
  if (str == NULL || nBins <= 0)
    return NULL;
  SPPoint** rgbHist = (SPPoint**)malloc(N * sizeof(SPPoint*));
  if (rgbHist == NULL)
    return NULL;

  // Read the image from the file.
  Mat image = imread(str, CV_LOAD_IMAGE_GRAYSCALE);
  float range[] = { 0, nBins };
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
  for (int i = 0; i<N; i++) {
    //instead of bgr(like the opencv example we use rgb for negeting cognitive dissonance 
    calcHist(&bgr_planes[N - i - 1], nImages, 0, Mat(), rgb_hists[i], 1, &nBins, &histRange);
    rgbHist[i] = spPointCreate(matrixToArray(&rgb_hists[i], nBins), nBins, imageIndex);
  }
  return rgbHist;
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

int main() {

  return 0;
}