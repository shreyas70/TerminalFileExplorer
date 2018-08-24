

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#include <stdio.h>
#include <sys/dir.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int arc, char ** argv){
    DIR *directory; // to open directory
    struct dirent *S_dirent; //dirent stucture
    struct stat S_stat;
    char *path="/media/sf_VB";
    directory=opendir(path);

    mode_t permission;

    while((S_dirent=readdir(directory))!=NULL){

       stat(S_dirent->d_name,&S_stat);
       permission = S_stat.st_mode;


       if(S_ISDIR(permission)){
         printf("d");
     }
     else{
        printf("-");
    }


    if( permission & S_IRUSR){
        printf("r");
    }
    else{
        printf("-");
    }
    if( permission & S_IWUSR){
        printf("w");
    }
    else{
        printf("-");
    }
    if( permission & S_IXUSR){
        printf("x");
    }
    else{
        printf("-");
    }


    if( permission & S_IRGRP){
        printf("r");
    }
    else{
        printf("-");
    }

    if( permission & S_IWGRP){
        printf("w");
    }
    else{
        printf("-");
    }
    if( permission & S_IXGRP){
        printf("x");
    }
    else{
        printf("-");
    }

    if( permission & S_IROTH){
        printf("r");
    }
    else{
        printf("-");
    }
    if( permission & S_IWOTH){
        printf("w");
    }
    else{
        printf("-");
    }
    if( permission & S_IXOTH){
        printf("x");
    }
    else{
        printf("-");
    }


    printf(" \t\t %d bytes ",S_stat.st_size);



    printf("\t %s ",S_dirent->d_name); 
    printf("\n\n");

    }
    closedir(directory);
    return 0;

}