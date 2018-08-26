
#include "ls.h"
#include <termios.h>
#include <stdio.h>


char root[PATH_MAX];

using namespace std;
int main(int argc, char const *argv[])
{
    //printf("\e[1;1H\e[2J"); //to clear screen
    //printf("\n");


    

    if (getcwd(root, sizeof(root)) == NULL){
    cout << "Fetching root Path error" << endl;
    }

    myLS(root); //prints initial directory
    //currentPath=root;
    char c; //used to fetch char
    FILE* input;





    //setting non C mode and disabling echo
    struct termios initialTerimalSettings, newTerimalSettings;
    input = fopen("/dev/tty", "r");
    tcgetattr(fileno(input), &initialTerimalSettings);
    newTerimalSettings = initialTerimalSettings;
    newTerimalSettings.c_lflag &= ~ICANON;
    newTerimalSettings.c_lflag &= ~ECHO;
    newTerimalSettings.c_cc[VMIN] = 1;
    newTerimalSettings.c_cc[VTIME] = 0;
    newTerimalSettings.c_lflag &= ~ISIG;


    //printf("\033[0;0H"); //move cursoe to initial position

    if(tcsetattr(fileno(input), TCSAFLUSH, &newTerimalSettings) != 0) { 
        cout << "switching on non C mode and disabling echo failed " << endl;
    }
    else{
        while(1){
            c= getc(input);
            if(c == 'q'){
                break;
            }
            else if(c=='\n' || c=='\r'){
                //char fullpath[PATH_MAX];

                showSelectedDir();
                //myLS(root+"showSelected"); 
                
            }
            else if(c=='\033'){
            getc(input); 
            switch(getc(input)) { 
                case 'A': //up
                    printf("\033[1A");
                    decrIndex();
                break;
                case 'B'://down
                   printf("\033[1B"); 
                   incrIndex();
                break;
                case 'C'://right
                    
                break;
                case 'D'://left
                    
                break;
            }
        }

    }
    tcsetattr(fileno(input), TCSANOW, &initialTerimalSettings);

}

return 0;
}

