
#include "utility.h"
#include <pwd.h>
#include <unistd.h>
#include <grp.h>
#include <ctime>
int myLS(char []);

void incrIndex();

void decrIndex();

void showSelectedDir();

int getCurrentViewTerminalLastRow();

void incrIndex();


int getIndex();

void decrIndex();

int getCurListLen();

int incrCurrentViewTopIndex();



void decrCurrentViewTopIndex();

int getCurrentViewTopIndex();

char* getCurrentPath();

void setScrollingFlag(int);


