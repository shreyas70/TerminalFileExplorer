
#include "ls.h"
#include <termios.h>
#include <stdio.h>

using namespace std;
int main(int argc, char const *argv[])
{
    printf("\e[1;1H\e[2J"); //to clear screen
    //printf("\n");
    myLS(); //prints initial directory
    char c;
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


    printf("\033[0;0H"); //move cursoe to initial position

    if(tcsetattr(fileno(input), TCSAFLUSH, &newTerimalSettings) != 0) { 
        cout << "switching on non C mode and disabling echo failed " << endl;
    }
    else{
        while(1){
            c= getc(input);
            if(c == 'u'){
                printf("\033[1A");
            }
            else if(c == 'd'){
                printf("\033[1B"); 
            }
            else if(c=='\n' || c=='\r'){
                break;
            }
            else if(c=='\033'){
            getc(input); 
            switch(getc(input)) { 
                case 'A': //up
                    printf("\033[1A");
                break;
                case 'B'://down
                   printf("\033[1B"); 
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