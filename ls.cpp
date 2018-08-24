

#include <sys/stat.h>
#include <sys/types.h>
#include <iomanip>
#include <ctime>
#include <sys/param.h>
#include <stdio.h>
#include <sys/dir.h>
#include <iostream>
#include <pwd.h>
#include <unistd.h>
#include <grp.h>
#include <stdlib.h>

using namespace std;

int main(int arc, char ** argv){
    DIR *directory; // to open directory
    struct dirent *S_dirent; //dirent stucture
    struct stat S_stat;
  

   char path[PATH_MAX];
   if (getcwd(path, sizeof(path)) == NULL){
    cout << "Current path error" << endl;
   }
   

    char fullpath [PATH_MAX];


    directory=opendir(path);

    mode_t permission;

    while((S_dirent=readdir(directory))!=NULL){

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, S_dirent->d_name);

        

        

        if (stat(fullpath,&S_stat) == -1) {
        //printf(stderr, "stat failed: %s\n", strerror(errno));
            printf("stat error");
            break;
        }

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

        
        time_t S_time=S_stat.st_ctime;



        struct passwd *S_password;
        S_password = getpwuid(S_stat.st_uid);



        printf(" \t %lld bytes ",S_stat.st_size);
    

        time_t t = S_stat.st_mtime;
        struct tm lt;
        localtime_r(&S_time, &lt);
        char timbuf[80];
        strftime(timbuf, sizeof(timbuf), "%c", &lt);

        cout << " " << timbuf << " ";
        cout << " " << S_password->pw_name << " ";

       // cout << " userID " << S_stat.st_uid << " ";

        struct group *g= getgrgid(S_stat.st_gid);
        //S_password = getpwuid(S_stat.st_gid);
        //cout << " " << S_password->pw_name << " ";
        cout << " " << g->gr_name << " " ;
        printf("\t %s ",S_dirent->d_name); 
        printf("\n\n");

    }
    closedir(directory);
    return 0;

}