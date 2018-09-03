
Note to TAs-
I have developed this application in MacOS environment. Certain calls like "xdg-open" are not available, therefore I have used equivalent calls. Kindly test the application on MacOS itself.

The entire application runs in non canoncial mode. Text wrapping is not handled.


References used to understand the concepts/techniques -
Beginning Linux Programing - 4th edition
XDG equivalent MacOS call - https://superuser.com/questions/911735/how-do-i-use-xdg-open-from-xdg-utils-on-mac-osx
https://www.mkompf.com/cplus/posixlist.html
http://man7.org/linux/man-pages/man2/stat.2.html
https://www.mkssoftware.com/docs/man1/mkdir.1.asp
https://www.geeksforgeeks.org/queue-cpp-stl/
https://linux.die.net/man/3/nftw
http://www.cplusplus.com/forum/beginner/189889/
http://www.cplusplus.com/reference/streambuf/streambuf/sbumpc/
http://www.cplusplus.com/reference/fstream/ifstream/ifstream/
http://pubs.opengroup.org/onlinepubs/009696699/functions/nftw.html
https://linux.die.net/man/3/strtok_r


___________________________________________


Description of the application
This application is a terminal-based file explorer. It is implemented strictly using low level system calls. No wrapper functions have been used, only POSXI standard calls. The applications considers directory in which it is run as the root directory. The application operates in 2 modes - normal mode and command mode. In normal mode the user can nagitave the directories through arrows keys and enter key. In command mode, the user can type commands and perform operations like move, copy, create, delete and rename (file as well as directories).


Implementation details:
-main.cpp contains all the initialization, taking input from user and calling required functions
-ls.cpp contains mainly the myLS() function and various other functions. processCmd() function parses the command types and calls required actions from cmd.cpp file
- cmd.cpp contains functions to copy directory, file, move, rename, create, etc. All these functions are called in ls.cpp when the command is being processed

-The history of the visited directories is stored in a string vector. 

-Vertical scrolling is handled in the following manner:
CurrentTopIndex and CurrentListLength is updated continuously.
 CurrentTopIndex - the index of the entry currently being displayed 1st on the screen

1. Only the number of enteries which fit in the terminal is printed each time
2. When the cursor reaches the last row, the application will check if there are further rows to be displayed, if yes, the entire screen is reprinted, starting from the 2nd top line => we get one new entry at the bottom
3. Similarly, when the cursor is at the top row and up arrow is pressed, the application checks if there are any items to be listed above, if yes, the entire screen is reprinted, with one new entry showing at the top.
4.  If CurrentTopIndex is 0, the cursor is in 1st row and up arrow is pressed, no action is needed. 
Similarly if the cursor is at the last row and current cursor index is equal to CurrentListLength, no action is need (already at the end of the list).


