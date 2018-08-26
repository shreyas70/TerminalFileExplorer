
#include "ls.h"
using namespace std;

char currentPath[PATH_MAX];

int indexx=0;
vector<string> vlist;

void incrIndex(){
    indexx++;
}

void decrIndex(){
    if(indexx>0){
        indexx--;
    }
    else{
        cout << "Error. Index is 0";
    }
}

// char* getCurrentPath(){
//     return currentPath;
// }

// char* setCurrentPath(char *a){
//     strcpy(currentPath,a);
//     //currentPath = a;

// }


int myLS(char path[]){ //lists current directory
    DIR *directory; // to open directory
    struct dirent *S_dirent; //dirent stucture
    struct stat S_stat;

    printf("\e[1;1H\e[2J"); //to clear screen
    indexx=0;
    vlist.clear();
  

   //char path[PATH_MAX];

   // if (getcwd(path, sizeof(path)) == NULL){
   //  cout << "Current Path error" << endl;
   // }


    char fullpath [PATH_MAX];
    directory=opendir(path);
    mode_t permission;
    // ** clear vector when traversing 
    //int i=0;
    while((S_dirent=readdir(directory))!=NULL){
        //update current path
        //currentPath=path;
        strcpy(currentPath,path);

        //consider adding full path here
        vlist.push_back(S_dirent->d_name);
        //i++;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, S_dirent->d_name);

        if (stat(fullpath,&S_stat) == -1) {
        //printf(stderr, "stat failed: %s\n", strerror(errno));
            printf("stat error");
            break;
        }

        //printing permission 
        {

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

        }

        struct passwd *S_password;
        S_password = getpwuid(S_stat.st_uid);

        printf(" \t %lld bytes ",S_stat.st_size);
    

        //time_t t = S_stat.st_mtime;
        time_t S_time=S_stat.st_ctime;
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
        printf("\n");

    }
    printf("\033[0;0H"); //move cursoe to initial position
    closedir(directory);
    return 0;
}


void showSelectedDir(){
    char fullpath [PATH_MAX];
    if (*currentPath == NULL){
    cout << "Current Path error in ls.cpp" << endl;
    }
    string s =  vlist[indexx];
    char temp[PATH_MAX];
    strcpy(temp, s.c_str()); 

    snprintf(fullpath, sizeof(fullpath), "%s/%s", currentPath, temp);
    myLS(fullpath);
}



//will clear the screen, update the things
int myLS_givenPath(){

}
