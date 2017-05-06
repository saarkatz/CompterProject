#ifndef SPGLOBALS_H_
#define SPGLOBALS_H_

/* Maximum size expected for strings in the program */
#define STRING_MAX_LENGTH 1024

/* Open file modes */
#define F_READ "r"
#define F_WRITE "w"

/* The flag representing the config value entry in command line */
#define CL_CONFIG_FLAG "-c"
#define CL_STOP_QUERYING "<>"

/* Default config file for the program */
#define DEFAULT_CONFIG "spcbir.config"

/* Feature files */
#define FEATURE_FILE_SUFFIX ".feats"
#define FEATURE_FILE_HEAD "%d %d %d"
#define FEATURE_FILE_LINE_VALUE "%lf"
#define FEATURE_FILE_LINE_END "\n"

/* Various messages retured by the program */
#define CL_DEFAULT_CONFIG_NO_OPEN_MESSAGE \
  "The default configuration file %s couldn’t be open\n"
#define CL_CONFIG_NO_OPEN_MESSAGE \
  "The configuration file %s couldn’t be open\n"
#define CL_USAGE_MESSAGE "Invalid command line : use -c <config_filename>\n"
#define CL_QUERY_MSG "Please enter an image path:\n"
#define CL_UNABLE_QUERY_IMAGE "Unable to use file '%s'\n"
#define CL_BEST_FOR_IMG "Best candidates for - %s - are:\n"
#define CL_CANDIDATE_IMG "%s\n"
#define CL_EXIT_MSG "Exiting...\n"

/* Error messages */
#define MSG_MEMORY_ALLOC_FAILURE "Memory allocation failure."
#define MSG_ERROR_GETTING_VALUE "Failed to retrive value from config."
#define MSG_CANT_OPEN_FILE "Unable to open file '%s'."
#define MSG_FAILED_TO_SAVE_FILE "Failed to save to file '%s'"
#define MSG_FAILED_TO_SEARCH_SIMIMG \
  "Failed to search for similar images to image '%s'."
#define MSG_FAILED_KDTREE "Failed to generate KDTree."
#define MSG_FAILED_KDARRAY "Failed to initialise kdArray."

/* Logger */
#define STDOUT_FILENAME "stdout"

/* Macro to handle the prints to the logger */
#define LOG_E(...) do {\
char __strbuff__[STRING_MAX_LENGTH]; \
sprintf(__strbuff__, __VA_ARGS__); \
spLoggerPrintError(__strbuff__, __FILE__, __func__, __LINE__); \
} while (0)

/**
Defenition of the .feats file format:
First line is the index of the image and the number of features in the file
and the size of each feature, each suppurated by a single space.
The following lines are as follows:
 - In the same line and saperated by spaces are all the values of the feature
   in order.
 - Finish with a new line.
*/

#endif // SPGLOBALS_H_
