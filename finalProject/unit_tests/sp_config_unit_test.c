#include <string.h>

#include "unit_test_util.h"

#include "../SPConfig.h"
#include "../SPLogger.h"
#include "../SPGlobals.h"

#define UT_LOGGER_FILENAME "logs/sp_kdtree_unit_test.log"
#define UT_LOGGER_LEVEL SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL

/* Test config */

#define NC_CONFIG_NAME NULL
#define ICN_CONFIG_NAME "configs/sp_config_test/INVALID_CONFIG_NAME_DONT_CREATE_THIS.config"
#define EC_CONFIG_NAME "configs/sp_config_test/empty.config"
#define AC_CONFIG_NAME "configs/sp_config_test/all_comments.config"
#define NID_CONFIG_NAME "configs/sp_config_test/no_image_directory.config"
#define NIP_CONFIG_NAME "configs/sp_config_test/no_image_prefix.config"
#define NIS_CONFIG_NAME "configs/sp_config_test/no_image_suffix.config"
#define NNOI_CONFIG_NAME "configs/sp_config_test/no_num_of_images.config"
#define PWS_CONFIG_NAME "configs/sp_config_test/parameters_with_spaces.config"
#define ISV_CONFIG_NAME "configs/sp_config_test/invalid_string_value.config"
#define IIVS_CONFIG_NAME "configs/sp_config_test/invalid_int_value_spaces.config"
#define IIVC_CONFIG_NAME "configs/sp_config_test/invalid_int_value_chars.config"
#define IBV_CONFIG_NAME "configs/sp_config_test/invalid_bool_value.config"
#define IEVS_CONFIG_NAME "configs/sp_config_test/invalid_enum_value_spaces.config"
#define IEVNV_CONFIG_NAME "configs/sp_config_test/invalid_enum_value_no_value.config"
#define DV_CONFIG_NAME "configs/sp_config_test/default_values.config"
#define VV_CONFIG_NAME "configs/sp_config_test/valid_values.config"
#define SV_CONFIG_NAME "configs/sp_config_test/scrambled_values.config"


bool testNullConfig() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(NC_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_INVALID_ARGUMENT == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidConfigName() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(ICN_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_CANNOT_OPEN_FILE == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testEmptyConfig() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(EC_CONFIG_NAME, &msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testAllCommets() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(AC_CONFIG_NAME, &msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testNoImageDirectory() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(NID_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_MISSING_DIR == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testNoImagesPrefix() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(NIP_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_MISSING_PREFIX == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testNoImagesSuffix() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(NIS_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_MISSING_SUFFIX == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testNoNumOfImages() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(NNOI_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_MISSING_NUM_IMAGES == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testParametersWithSpaces() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(PWS_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_INVALID_STRING == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidStringValue() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(ISV_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_INVALID_STRING == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidIntValueSpaces() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(IIVS_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_INVALID_INTEGER == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidIntValueChars() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(IIVC_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_INVALID_INTEGER == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidBoolValue() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(IBV_CONFIG_NAME, &msg);
  ASSERT_FALSE(SP_CONFIG_SUCCESS == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidEnumValueSpaces() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(IEVS_CONFIG_NAME, &msg);
  ASSERT_FALSE(SP_CONFIG_SUCCESS == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testInvalidEnumValueNoValue() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  config = spConfigCreate(IEVNV_CONFIG_NAME, &msg);
  ASSERT_FALSE(SP_CONFIG_SUCCESS == msg);
  ASSERT_TRUE(NULL == config);
  return true;
}

bool testDefaultValues() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  char buffer[STRING_MAX_LENGTH];

  /* To make sure buffer is zero terminated */
  buffer[STRING_MAX_LENGTH - 1] = '\0';

  config = spConfigCreate(DV_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);
  ASSERT_FALSE(NULL == config);

  msg = spConfigGetImagePath(buffer, config, -1);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  msg = spConfigGetImagePath(buffer, config, 0);
  ASSERT_TRUE(0 == strcmp("./images/img0.jpg", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = spConfigGetImagePath(buffer, config, 1);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(20 == spConfigGetPCADim(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetPCAPath(buffer, config);
  ASSERT_TRUE(0 == strcmp("./images/pca.yml", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(100 == spConfigGetNumOfFeatures(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(true == spConfigIsExtractionMode(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(1 == spConfigGetNumSimilarImages(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(1 == spConfigGetKNN(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(MAX_SPREAD == spConfigGetSplitMethod(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(false == spConfigIsMinimalGui(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(3 == spConfigGetLoggerLevel(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetLoggerFilename(buffer, config);
  ASSERT_TRUE(0 == strcmp("stdout", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  spConfigDestroy(config);
  return true;
}

bool testValidValues() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  char buffer[STRING_MAX_LENGTH];

  /* To make sure buffer is zero terminated */
  buffer[STRING_MAX_LENGTH - 1] = '\0';

  config = spConfigCreate(VV_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);
  ASSERT_FALSE(NULL == config);

  msg = spConfigGetImagePath(buffer, config, -1);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  msg = spConfigGetImagePath(buffer, config, 7);
  ASSERT_TRUE(0 == strcmp("./pictures/pic7.bmp", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = spConfigGetImagePath(buffer, config, 11);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  ASSERT_TRUE(15 == spConfigGetPCADim(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetPCAPath(buffer, config);
  ASSERT_TRUE(0 == strcmp("./pictures/myPca.friend", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(246 == spConfigGetNumOfFeatures(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(false == spConfigIsExtractionMode(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(6 == spConfigGetNumSimilarImages(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(13 == spConfigGetKNN(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(RANDOM == spConfigGetSplitMethod(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(true == spConfigIsMinimalGui(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(1 == spConfigGetLoggerLevel(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetLoggerFilename(buffer, config);
  ASSERT_TRUE(0 == strcmp("myLoggerfile.loog", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  spConfigDestroy(config);
  return true;
}

bool testScrambledValues() {
  SPConfig config;
  SP_CONFIG_MSG msg;
  char buffer[STRING_MAX_LENGTH];

  /* To make sure buffer is zero terminated */
  buffer[STRING_MAX_LENGTH - 1] = '\0';

  config = spConfigCreate(SV_CONFIG_NAME, &msg);
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);
  ASSERT_FALSE(NULL == config);

  msg = spConfigGetImagePath(buffer, config, -1);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  msg = spConfigGetImagePath(buffer, config, 3);
  ASSERT_TRUE(0 == strcmp("./gallery/photo3.jpg", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = spConfigGetImagePath(buffer, config, 7);
  ASSERT_TRUE(SP_CONFIG_INDEX_OUT_OF_RANGE == msg);

  ASSERT_TRUE(17 == spConfigGetPCADim(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetPCAPath(buffer, config);
  ASSERT_TRUE(0 == strcmp("./gallery/pca1.txt", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(200 == spConfigGetNumOfFeatures(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(false == spConfigIsExtractionMode(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(3 == spConfigGetNumSimilarImages(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(15 == spConfigGetKNN(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(INCREMENTAL == spConfigGetSplitMethod(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(true == spConfigIsMinimalGui(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  ASSERT_TRUE(2 == spConfigGetLoggerLevel(config, &msg));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  msg = SP_CONFIG_ALLOC_FAIL;
  msg = spConfigGetLoggerFilename(buffer, config);
  ASSERT_TRUE(0 == strcmp("log", buffer));
  ASSERT_TRUE(SP_CONFIG_SUCCESS == msg);

  spConfigDestroy(config);
  return true;
}

int main() {
  /* Declare logger */
  SP_LOGGER_MSG msg = spLoggerCreate(UT_LOGGER_FILENAME, UT_LOGGER_LEVEL);
  if (SP_LOGGER_DEFINED != msg && SP_LOGGER_SUCCESS != msg) {
    PRINT_E("Unable to create logger, aborting test suite.\n");
    return -1;
  }

  RUN_TEST(testNullConfig);
  RUN_TEST(testInvalidConfigName);
  RUN_TEST(testEmptyConfig);
  RUN_TEST(testAllCommets);
  RUN_TEST(testNoImageDirectory);
  RUN_TEST(testNoImagesPrefix);
  RUN_TEST(testNoImagesSuffix);
  RUN_TEST(testNoNumOfImages);
  RUN_TEST(testParametersWithSpaces);
  RUN_TEST(testInvalidStringValue);
  RUN_TEST(testInvalidIntValueSpaces);
  RUN_TEST(testInvalidIntValueChars);
  RUN_TEST(testInvalidBoolValue);
  RUN_TEST(testInvalidEnumValueSpaces);
  RUN_TEST(testInvalidEnumValueNoValue);
  RUN_TEST(testDefaultValues);
  RUN_TEST(testValidValues);
  RUN_TEST(testScrambledValues);

  spLoggerDestroy();
  return 0;
}
