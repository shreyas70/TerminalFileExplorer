
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

int main(){
    char test[80], blah[80];
     char *sep = " ";
     char *substring;

     strcpy(test, "copy file1 file2");
     //string test="sadlfjsadf  sdaflj";
      int i=0;

     for (substring = strtok(test, sep); substring; substring = strtok(NULL, sep), i++)
     {
        if(i==0){
            cout << " cmd " << substring;
        }
        else if (i==1){
            cout << " 1 " << substring;
        }
        else if(i==2){
            cout << "2 " << substring;
        }
             //printf("So far we're at %s\n", substring);
    
     }

return 1;
}