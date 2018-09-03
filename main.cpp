
#include "ls.h"
#include <termios.h>
#include <stdio.h>
using namespace std;



char root[PATH_MAX];

string cmd_buffer="";
int mode=0; //start in normal mode

string getCmdBuffer(){
    return cmd_buffer; //used in cmd mode
}

int getMode(){
    return mode; //normal or cmd mode bit
}

void clearCmdBuffer(){
    cmd_buffer=": "; //the string typed in cmd mode will be in this buffer
}

char * getRoot(){
    return root; //will store the path in which the application is launched
} 



int main(int argc, char const *argv[])
{
    if (getcwd(root, sizeof(root)) == NULL){
        cout << "Fetching root Path error" << endl;
    }
    initializeHistory();
    addToHistory(root);
    myLS(root); //prints initial directory
    char c; //used to fetch char

    FILE* input;
    //setting non Canonical mode and disabling echo
    struct termios initialTerimalSettings, newTerimalSettings;
    input = fopen("/dev/tty", "r");
    tcgetattr(fileno(input), &initialTerimalSettings);
    newTerimalSettings = initialTerimalSettings;
    newTerimalSettings.c_lflag &= ~ICANON;
    newTerimalSettings.c_lflag &= ~ECHO;
    newTerimalSettings.c_cc[VMIN] = 1;
    newTerimalSettings.c_cc[VTIME] = 0;
    newTerimalSettings.c_lflag &= ~ISIG;

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
                //Enter is pressed
                showSelectedDir();
            }
            else if(c=='h' || c=='H'){
                myLS(root); //display root dir

            }
            else if(c==127){
                //need to go one dir up
                backSpacePressed();
            }
            //arrow key
            else if(c=='\033'){ 
                getc(input); 
                switch(getc(input)) { 
                case 'A': //up
                    if(getIndex()==0)
                    {
                            //we are at the top, first item
                            //do nothing.
                    }
                    else if(getIndex()==getCurrentViewTopIndex())
                    {
                        //currently at the top but there are few entries above this
                        decrCurrentViewTopIndex();
                        decrIndex();
                        setScrollingFlag(2); //traversing up
                        char temp[PATH_MAX];
                        myLS(temp); //this call will refresh the screen acccordingly
                    }
                    else //if cursor is in the middle of the list go up
                    {
                        printf("\033[1A");
                        decrIndex(); //cursor index is decrmenteds
                    }
                    break;

                case 'B'://down

                    if(getIndex()==(getCurListLen()-1)){
                         //at the last item
                         // do nothing

                    }
                    else{
                        //checking if cursor is at the end of the terminal

                        int top = getCurrentViewTopIndex();
                        int bottom = getIndex();

                        if(bottom - top == getCurrentViewTerminalLastRow()){
                            //at last row => More entries are there to display
                            incrCurrentViewTopIndex();
                            incrIndex();
                            setScrollingFlag(1);
                            char temp[PATH_MAX];
                            myLS(temp);

                        }
                        else{
                           printf("\033[1B"); 
                           incrIndex();

                       }

                   }
                   break;
                case 'C'://right

                    // will traverse the history stored and call ls on in
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
                cmd_buffer+=": ";
                mode=1; //setting cmd mode
                enterCmdMode();
                while(1){
                    c= getc(input);
                    if(c==27)
                    { //esc pressed
                        mode=0; 
                        cmd_buffer="";
                        exitCmdMode();
                        break;
                    }
                    else if(c==127){
                        //cout << "delete pressed!"
                        if(cmd_buffer.size()>2){
                        cmd_buffer=cmd_buffer.substr(0,cmd_buffer.size()-1);
                        enterCmdMode();
                         }

                    }
                    else if(c=='\n'){
                        //cout << "enter pressed!";
                        processCmd(cmd_buffer);
                        cmd_buffer=": "; //clearing
                        enterCmdMode(); //torefresh

                    }
                    else{
                        cmd_buffer+=c;
                        enterCmdMode();
                    }
                }
            }
        }
    }
    //settin back original settings, reseting terminal behaviour
    tcsetattr(fileno(input), TCSANOW, &initialTerimalSettings);
return 0;
}





