// This program implements a run_tests command (sould be compiled to run_tests)
// Usage:
//  ./run_tests PATH [-s SUFFIX] [-t TIMEOUT]
// The program iterates over all the files within the directory specified by
// PATH and attemps to run every file that ends with SUFFIX (defaults to 
// 'unit_test'). 
// The runtime of the tests is limited by TIMEOUT seconds. If TIMEOUT is set 
// to 0, then the runtime of the tests in unlimited. TIMEOUT defaults to 60.

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>

// dirent.h does not ship with visual studio! for more information see 
// https://www.gnu.org/software/libc/manual/html_node/Opening-a-Directory.html
#include <dirent.h> 

#define USAGE_MSG "Try './run_tests PATH [-s SUFFIX] [-t TIMEOUT]'"
#define ZERO_TIMEOUT 60

// Credit http://stackoverflow.com/users/20481/plinth at
// http://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
int ends_with(const char *str, const char *suffix)
{
  size_t lenstr;
  size_t lensuffix;

  if (!str || !suffix)
    return 0;
  lenstr = strlen(str);
  lensuffix = strlen(suffix);
  if (lensuffix > lenstr)
    return 0;
  return 0 == strncmp(str + lenstr - lensuffix, suffix, lensuffix);
}

int main(int argc, char **argv) {
  char filename[1024];
  char suffix[256] = "unit_test";
  int timeout = ZERO_TIMEOUT;

  pid_t p;
  int wstatus;
  int status;

  int i;

  char str[256];
  char *in_args[] = { str, NULL };

  // for directory iteration.
  DIR *dir;
  struct dirent *ent;
  struct stat *file_info;

  // Make sure that enough argument are given.
  if (argc < 2) {
    printf(USAGE_MSG);
    return 1;
  }

  // Get parameters
  for (i = 2; i < argc; i++) {
    if (0 == strcmp("-s", argv[i])) {
      i += 1;
      if (i >= argc) {
        printf(USAGE_MSG);
        return 1;
      }
      strcpy(suffix, argv[i]);
    }
    else if (0 == strcmp("-t", argv[i])) {
      i += 1;
      if (i >= argc) {
        printf(USAGE_MSG);
        return 1;
      }
      timeout = atoi(argv[i]);
    }
    else {
      printf(USAGE_MSG);
      return 1;
    }
  }

  // Iterate over files in directory
  // Credit http://stackoverflow.com/users/23264/peter-parker at
  // http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c++
  if ((dir = opendir(argv[1])) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      strcpy(filename, argv[1]);
      if (!ends_with(filename, "/")) {
        strcpy(filename + strlen(filename), "/");
      }
      strcpy(filename + strlen(filename), ent->d_name);
      // Note that not all filesystems support the field d_type of the dirent
      // object, but ext4 does.
      switch (ent->d_type) {
      case DT_UNKNOWN: // If d_type is not supported. Use stat() instead.
        printf("d_type not supported. Using stat instead\n");
        i = stat(filename, file_info);
        if (errno != 0) {
          // An error occurred
          perror(strerror(errno));
          return 1;
        }
        if (!(file_info->st_mode & S_IFREG)) {
          // If this is not a regular file, break
          break;
        }
      case DT_REG: // Regular file (does not include sym-links or other things)
        if (ends_with(ent->d_name, suffix)) {
          // This is a test, run it.
          if (-1 == (p = fork())) {
            printf("Someting went wrong!\n");
            return 1;
          }
          else if (0 == p) {
            // We are the child
            strcpy(in_args[0], ent->d_name);
            printf("Running \"%s\":\n", in_args[0]);
            execv(filename, in_args);
            printf("Failed to execute \"%s\".\n", filename);
            return 1;
          }
          // We are the parent
          if (timeout > 0) {
            // Busy-sleeping might not be the best solution but is surely the
            // simplest one.
            for (i = 0; i < timeout && 0 == (status = waitpid(p, &wstatus, WNOHANG)); i++) {
              sleep(1);
            }
            if (i = timeout && 0 == status) {
              // code timed out, kill it
              if (kill(p, SIGKILL) < 0) {
                printf("Error while killing procces (%d): %s\n", p,
                  strerror(errno));
              }
            }
          }
          else {
            status = waitpid(p, &wstatus, 0);
          }
          if (0 == status) {
            printf("\"%s\" timed out after %d seconds.\n", ent->d_name, timeout);
          }
          else if (!WIFEXITED(wstatus)) {
            if (WIFSIGNALED(wstatus)) {
              // Not all systems define WCOREDUMP
#ifdef WCOREDUMP
              if (WCOREDUMP(wstatus)) {
                printf("Segmentaion fault occured during run of \"%s\".\n",
                  ent->d_name);
              }
              else {
#else
              printf("A major error occured during run of \"%s\" and it was "
                "terminated by a signal (%d).\n", ent->d_name, WTERMSIG(wstatus));
#endif
#ifdef WCOREDUMP
              }
#endif
            }
            else {
              printf("Run of \"%s\" terminated erroneously but not by a "
                "signal.\n", ent->d_name);
            }
          }
          else {
            if (WEXITSTATUS(wstatus) != 0) {
              printf("A minor error occured during run of \"%s\" and it "
                "exited with status %d.\n", ent->d_name, WEXITSTATUS(wstatus));
            }
            else {
              printf("Run finished with exit status %d.\n", WEXITSTATUS(wstatus));
            }
          }
        }
      }
    }
    if (errno != 0) {
      // An error occurred
      perror(strerror(errno));
      return 1;
    }
    closedir(dir);
  }
  else {
    /* could not open directory */
    perror(strerror(errno));
    return 1;
  }
}
