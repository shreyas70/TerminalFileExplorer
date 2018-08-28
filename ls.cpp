
#include "ls.h"
using namespace std;


char currentPath[PATH_MAX];

int curListLen=0;
int currentViewTopIndex=0;
int indexx=0; //the file on which cursor is present
vector<string> vlist;
int scrollingFlag=0; //1 for down,2  for up
//list
int currentViewTerminalLastRow=0;


void setScrollingFlag(int a){
    scrollingFlag=a; //1 for down, 2 for up
}
int getCurrentViewTerminalLastRow(){
    return currentViewTerminalLastRow;
}

void incrIndex(){
    indexx++;
}

int getIndex(){
    return indexx;
}

void decrIndex(){
    if(indexx>0){
        indexx--;
    }
    else{
        cout << "Error. Index is 0";
    }
}

int getCurListLen(){
    return curListLen;
}
int incrCurrentViewTopIndex(){
    if(currentViewTopIndex>=0){
        currentViewTopIndex++;
       // cout << "top indrecmented";
        return 1;
    }
    else{
        return 0;
    }
}

void decrCurrentViewTopIndex(){
    currentViewTopIndex--;
    //cout << "top indrecmented";
}

int getCurrentViewTopIndex(){
    return currentViewTopIndex;
}

char* getCurrentPath(){
    return currentPath;
}


// char* getCurrentPath(){
//     return currentPath;
// }

// char* setCurrentPath(char *a){
//     strcpy(currentPath,a);
//     //currentPath = a;

// }


int myLS(char path[]){ //lists current directory

    cout << "ls called!" << endl;
    DIR *directory; // to open directory
    struct dirent *S_dirent; //dirent stucture
    struct stat S_stat;

    printf("\e[1;1H\e[2J"); //to clear screen
    

    if(!scrollingFlag){
        indexx=0;
        currentViewTopIndex=0;

    }else{
        //do nothing
        //path is current path
       path=currentPath;
       strcpy(currentPath,path);
       cout << "works! top " <<currentViewTopIndex <<endl;

    }

    vlist.clear();

    


   //char path[PATH_MAX];

   // if (getcwd(path, sizeof(path)) == NULL){
   //  cout << "Current Path error" << endl;
   // }


    char fullpath [PATH_MAX];
    directory=opendir(path);
    mode_t permission;
    // ** clear vector when traversing 
    int i=0;
    struct winsize S_windowsize;
    ioctl(0, TIOCGWINSZ, &S_windowsize);
    //cout << S_windowsize.ws_col << endl;
    while((S_dirent=readdir(directory))!=NULL){
        
        //update current path
        //currentPath=path;
        strcpy(currentPath,path);

        //consider adding full path here
        vlist.push_back(S_dirent->d_name);
        //i++;
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, S_dirent->d_name);

        if(i>=currentViewTopIndex && i<((S_windowsize.ws_row)-1+currentViewTopIndex)){ 
            //print only if visible. 
            //Otherwise just insert and find max length

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

        i++;

    }
    //cout << "while loop exited with i = " << i << endl;
    curListLen=vlist.size();
    currentViewTerminalLastRow=S_windowsize.ws_row-2;

    if(!scrollingFlag){ //new directory
        printf("\033[0;0H"); //move cursoe to initial position
    }
    else{ 
        //scrolling. keep pointer there, do nothing
        
        if(scrollingFlag==1) // scrolling down
            printf("\033[%d;0H",currentViewTerminalLastRow+1);
        else if(scrollingFlag==2) // scrolling down
            printf("\033[0;0H");

        scrollingFlag=0;

    }
    

    closedir(directory);
    return 0;
}


void showSelectedDir(){

    char fullpath [PATH_MAX];
    if (*currentPath == NULL){
        cout << "Current Path error in ls.cpp" << endl;
    }
    string s =  vlist[indexx];
    char fileName[PATH_MAX]; //will have the name of the file
    strcpy(fileName, s.c_str()); 

    snprintf(fullpath, sizeof(fullpath), "%s/%s", currentPath, fileName);

    struct stat S_stat;
    if (stat(fullpath,&S_stat) == -1) {
        //printf(stderr, "stat failed: %s\n", strerror(errno));
        printf("stat error");

    }

        //printing permission 



    mode_t permission = S_stat.st_mode;

    if(!S_ISDIR(permission)){
        int pid = fork();
        if (pid == 0) {

          execl("/usr/bin/open", "open", fullpath, (char *)0);
          exit(1);
      }
  }
  else{


    myLS(fullpath);

}
}



//will clear the screen, update the things
// int myLS_givenPath(){

// }
