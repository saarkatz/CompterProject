
  double* array = (double*)malloc(m->rows * sizeof(double));
  double* array = (double*)malloc(m->cols * sizeof(double));
  SPPoint** rgbHist = (SPPoint**)malloc(N * sizeof(SPPoint*));
  SPPoint** pointArray = (SPPoint**)malloc(ds1.rows*sizeof(SPPoint*));
  BPQueueElement* peekElement = (BPQueueElement*)malloc(sizeof(BPQueueElement)); //maybe
  int* indexResult = (int*)malloc(kClosest * sizeof(int));
  globalArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  localArray = (SPPoint***)malloc(numOfImages * sizeof(SPPoint**));
  featureSizes = (int*)malloc(numOfImages * sizeof(int));
  totalMatches = (SPImageCounter*)malloc(numOfImages * sizeof(SPImageCounter));
  resultArray = (int*)malloc(K_CLOSEST * sizeof(int));
  int* result = (int*)malloc(kClosest * sizeof(int));
  SPHist* histDistanceArray = (SPHist*)malloc(numOfImages * sizeof(SPHist));




  free(data);
  free(peekElement);//maybe
  free(resultArray);
  free(resultArray);
  free(resultArray);
  free(queryImageHistogram);
  free(queryImageFeatures);
  free(histDistanceArray);
  free(globalArray);
  free(localArray);
  free(featureSizes);
