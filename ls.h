
#include "utility.h"
#include <pwd.h>
#include <unistd.h>
#include <grp.h>
#include <ctime>
#include <vector>

void leftArrowPressed();

int myLS(char []);

void incrIndex();

void decrIndex();

void showSelectedDir();

void initializeHistory();

void enterCmdMode();

void addToHistory(char []);

int getCurrentViewTerminalLastRow();

void rightArrowPressed();

void resetTraverseFlag();

void incrIndex();


int getIndex();

void decrIndex();

int getCurListLen();

int incrCurrentViewTopIndex();



void decrCurrentViewTopIndex();

int getCurrentViewTopIndex();

char* getCurrentPath();

void setScrollingFlag(int);




