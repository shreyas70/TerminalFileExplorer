#include "utility.h"
#include <fstream>      
#include <cstdio>       
#include <fcntl.h>
#include <unistd.h>
#include <queue>
#include <ftw.h>
#include <string.h>

int singleFileCopy ( char * source,  char* destination) ;

int createFile(char * path1);

int createDir(char *path);

int delete_file(char *path);

int delete_empty_dir(char *path);

int moveFile(char * path1, char* path2);

int snapShot(char * path, int outputFD);

int snapShotHelper(char * path, char* filename);

int delete_dir(char * path);

int copy_dir(char * path1, char * destination);