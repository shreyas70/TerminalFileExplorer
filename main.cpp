
#include "ls.h"
#include <termios.h>
#include <stdio.h>



char root[PATH_MAX];

string cmd_buffer="";
int mode=0; //start in normal mode

string getCmdBuffer(){
    return cmd_buffer;
}

int getMode(){
    return mode;
}


char * getRoot(){
    return root;
}



using namespace std;
int main(int argc, char const *argv[])
{
    //printf("\e[1;1H\e[2J"); //to clear screen
    //printf("\n");




    if (getcwd(root, sizeof(root)) == NULL){
        cout << "Fetching root Path error" << endl;
    }
    initializeHistory();

    addToHistory(root);
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
        //normal mode looop
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
            else if(c=='h' || c=='H'){
                myLS(root);

            }
            //arrow key
            else if(c=='\033'){ 
                getc(input); 
                switch(getc(input)) { 
                case 'A': //up
                if(getIndex()==0){
                        //we are at the top, first item
                        //do nothing.
                }
                else if(getIndex()==getCurrentViewTopIndex()){

                        //currently at the top but there are few entries above this
                    decrCurrentViewTopIndex();
                    decrIndex();
                    setScrollingFlag(2);
                    myLS("");
                }
                else{
                    printf("\033[1A");
                    decrIndex();
                }
                break;

                case 'B'://down

               // cout << "hi" << endl;
                 //printf("%s\n","came in works!" ); 
                if(getIndex()==(getCurListLen()-1)){
                     //at the last item
                     // do nothing
                    //cout << "last item" ;

                }
                else{
                    //checking if cursor is at the end of the terminal

                    int top = getCurrentViewTopIndex();
                    int bottom = getIndex();

                    //cout << top << " " <<
                    if(bottom - top == getCurrentViewTerminalLastRow()){
                        //cout << "works!" <<endl;
                        //at last row. More entries are there to display



                        incrCurrentViewTopIndex();
                        incrIndex();
                        setScrollingFlag(1);
                        // cout << getCurrentPath() << endl;
                        myLS("");

                    }
                    else{

                        //cout << "not works!" <<endl;

                       printf("\033[1B"); 
                       incrIndex();

                   }

               }
               break;
                    case 'C'://right
                    rightArrowPressed();
                    


                    break;
                    case 'D'://left
                    //traverseFlag
                    leftArrowPressed();


                    break;
                }
            }
                //cmd mode
            else if(c==':'){
                cmd_buffer+=":";
                mode=1; //setting cmd mode
                enterCmdMode();
              
                while(1){
                    c= getc(input);
                    if(c==27){
                        cout << "esc!";
                        mode=0; 
                        break;
                    }else{
                        cmd_buffer+=c;
                        enterCmdMode();
                    }
                }

            }
        }


    }
    tcsetattr(fileno(input), TCSANOW, &initialTerimalSettings);
return 0;
}







// void switchToCMDMode(){
//     FILE* input;
//     struct termios initialTerimalSettings, newTerimalSettings;
//     cout << "switcing to cmd " <<endl;
//     input = fopen("/dev/tty", "r");
//     tcgetattr(fileno(input), &initialTerimalSettings);
//     newTerimalSettings = initialTerimalSettings;
//     newTerimalSettings.c_lflag &= ECHO;
//     if(tcsetattr(fileno(input), TCSAFLUSH, &newTerimalSettings) != 0) { 
//         cout << "switching to CMD mode failed " << endl;
//     }
//     return;
// }

// void switchToNormalMode(){
//     FILE* input;
//     struct termios initialTerimalSettings, newTerimalSettings;
//     cout << "switcing to cmd " <<endl;
//     input = fopen("/dev/tty", "r");
//     tcgetattr(fileno(input), &initialTerimalSettings);
//     newTerimalSettings = initialTerimalSettings;
//     newTerimalSettings.c_lflag &= ~ECHO;
//     if(tcsetattr(fileno(input), TCSAFLUSH, &newTerimalSettings) != 0) { 
//         cout << "switching to normal mode failed " << endl;
//     }
//     return;
// }

