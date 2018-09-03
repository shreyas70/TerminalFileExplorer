
#include "ls.h"
using namespace std;

struct history
{
	vector<string> list;
	int curIndex;
};


int curListLen=0;
int currentViewTopIndex=0;
int traverseFlag=0;
int cursorIndex=0; //the file on which cursor is present
vector<string> directoryItems; //list is directories
int scrollingFlag=0; //1 for down,2  for up
int gotoFlag;
int currentViewTerminalLastRow=0;

struct history dirHistory;
void initializeHistory(){
	dirHistory.curIndex=-1;
}


//to add to history when new directory is visited
void addToHistory(char path[]){
	string s_path=string(path);
	dirHistory.list.push_back(s_path);
	dirHistory.curIndex++;
}

void setScrollingFlag(int a){
	scrollingFlag=a; //1 for down, 2 for up. 
	//To be able to differentiate up whether to move the list up or down

}
int getCurrentViewTerminalLastRow(){
	//index of the file that is displayed on the last row currently
	return currentViewTerminalLastRow;
}

void setTraverseFlag(){
	//travsing through the history using arrow keys
	traverseFlag=1; 
}

void resetTraverseFlag(){
	traverseFlag=0; 
}

void incrIndex(){
	cursorIndex++;
}

int getIndex(){
	return cursorIndex;
}

void decrIndex(){
	if(cursorIndex>0){
		cursorIndex--;
	}
	else{
		cout << "Error. Index is 0";
	}
}

int getCurListLen(){
	return curListLen;
}

int incrCurrentViewTopIndex(){
	//when the list needs to be refreshed on down arrow at last row
	if(currentViewTopIndex>=0){
		currentViewTopIndex++;
		return 1;
	}
	else{
		return 0;
	}
}

void decrCurrentViewTopIndex(){
	//when the list needs to be refreshed on up arrow at first row
	currentViewTopIndex--;
}

int getCurrentViewTopIndex(){
	return currentViewTopIndex;
}


//void printStatusBar(S_windowsize.ws_row)-2



int myLS(char path[PATH_MAX]){ //lists given directory with properties

	if(scrollingFlag){ //scroling with up and down => current dir is same
		strcpy(path, dirHistory.list[dirHistory.curIndex].c_str());
	}

	
	DIR *directory; // to open directory

	char fullpath [PATH_MAX];
	directory=opendir(path);
	if(directory==NULL)
		return -1;


	if(!traverseFlag && !getMode() && !scrollingFlag){ //not traversing and not in cmd mode
		//inserting to history
		if(cursorIndex!=0 && cursorIndex!=1){ // if . or .. is not pressed
			// addign to history
			string s_path=string(path);
			dirHistory.list.push_back(s_path);
			dirHistory.curIndex++;
		}
		else if(cursorIndex==1){ // .. is pressed
			dirHistory.curIndex--;

		}

	}

	
	struct dirent *S_dirent; //dirent stucture
	struct stat S_stat;
	printf("\e[1;1H\e[2J"); //to clear screen
	directoryItems.clear();

	if( getMode() && gotoFlag)
	{ //if in cmd mode and using goto option
		//inserting to history
		string s_path=string(path);
		dirHistory.list.push_back(s_path);
		dirHistory.curIndex++;
	}

	

	if(!scrollingFlag) //reset indicies if not in scrolling mode
	{
		cursorIndex=0;
		currentViewTopIndex=0;
	}

	mode_t permission;
	int i=0; //used to implement scrolling. index Variable for directory items
	struct winsize S_windowsize; //used to fetch terminal size
	ioctl(0, TIOCGWINSZ, &S_windowsize);

	//S_dirent will have item name and inode 
	while((S_dirent=readdir(directory))!=NULL){

		//directoryItems will contain the list of items of the currently visibly directory
		directoryItems.push_back(S_dirent->d_name);
		snprintf(fullpath, sizeof(fullpath), "%s/%s", path, S_dirent->d_name);

		if(i>=currentViewTopIndex && i<((S_windowsize.ws_row)-2+currentViewTopIndex))
		{ //to print only as many rows that can be viewed given teriminal size

			//stat is used to fetch details about each directory item
			if (stat(fullpath,&S_stat) == -1) { 
				printf("stat error");
				break;
			}

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

			//printing size
			printf(" \t %lld bytes ",S_stat.st_size);

			//printing modified time
			time_t S_time=S_stat.st_ctime;
			struct tm lt;
			localtime_r(&S_time, &lt);
			char timbuf[80];
			strftime(timbuf, sizeof(timbuf), "%c", &lt);
			cout << " " << timbuf << " ";

			//printing user name fetched from /usr/bin/passwd file
			struct passwd *S_password;
			S_password = getpwuid(S_stat.st_uid);
			cout << " " << S_password->pw_name << " ";

			//printing group
			struct group *g= getgrgid(S_stat.st_gid);
			cout << " " << g->gr_name << " " ;
			printf("\t %s ",S_dirent->d_name); 
			printf("\n");
		}
		i++;
	}

	


	curListLen=directoryItems.size(); //used to implement scrolling
	currentViewTerminalLastRow=S_windowsize.ws_row-3;

	if(getMode()){
		cout << "\e[1mCommand Mode\e[0m" <<endl;
	}
	else{
		cout << "\e[1mNormal Mode\e[0m" <<endl ;
	}

	if(!scrollingFlag && !getMode()){ //new directory entered
		printf("\033[0;0H"); //move cursor to initial position
	}
	else if(getMode()){
		//print string if cmd mode is activated
		cout << getCmdBuffer() ;
		//leave cursor there

	}
	else{ //scrolling
		if(scrollingFlag==1) // scrolling down.
			printf("\033[%d;0H",currentViewTerminalLastRow+1);
		else if(scrollingFlag==2) // scrolling up
			printf("\033[0;0H");

		scrollingFlag=0;
	}

	

	//printStatusBar();
	

	closedir(directory);
	return 0;
}

//when enter is pressed
void showSelectedDir(){

	//if .. is pressed in root dir nothing should happen
	string s = dirHistory.list[dirHistory.curIndex];
	if(strcmp(s.c_str(),getRoot())==0 && cursorIndex==1)
		return;

	//clearing if there is any unwanted stuff in history
	if(!traverseFlag){ 
		if(dirHistory.curIndex < dirHistory.list.size()-1){
			dirHistory.list.erase(dirHistory.list.begin() + dirHistory.curIndex+1 ,dirHistory.list.end());
		}
	}


	char fullpath [PATH_MAX];
	s =  directoryItems[cursorIndex];
	char fileName[PATH_MAX]; //will have the name of the file
	strcpy(fileName, s.c_str()); 


	//current file
	s = dirHistory.list[dirHistory.curIndex];
	snprintf(fullpath, sizeof(fullpath), "%s/%s",s.c_str(), fileName);



	struct stat S_stat;
	if (stat(fullpath,&S_stat) == -1)
	{
		printf("stat error");
	}


	//fetching permission 
	mode_t permission = S_stat.st_mode;



	//if it is a file, open it
	if(!S_ISDIR(permission))
	{
		int pid = fork();
		if (pid == 0) {
			execl("/usr/bin/open", "open", fullpath, (char *)0);
			exit(1);
		}
	}
	else //open it if it is a directory
	{
		myLS(fullpath);

	}
}


//used to process entered command. 
//cmd tokens will contains the string tokens in the command
vector<string> cmdTokens;
vector<string> cmdTokensAbs;

void processCmd(string cmd){ //will be called when enter is pressed in cmd mode
	char c_cmd[PATH_MAX];
	char sep[] = " ";
	char *substring;

	strcpy(c_cmd, cmd.c_str());
	int j=0;

	for (substring = strtok(c_cmd, sep); substring; substring = strtok(NULL, sep), j++) {
		if(j!=0)
		{ //don't push token ':'
			cmdTokens.push_back(string(substring));
		}
	}
   convToAbsolute(); //updates the vector cmdTokens into absolute paths


   //proccessing cmd
   if(cmdTokens[0].compare("copy")==0)
   {
   	for(int i=1;i<cmdTokens.size()-1;i++)
   	{
		//to check if it is a directory or file
   		struct stat S_stat;
   		stat(cmdTokensAbs[i].c_str(),&S_stat);
   		mode_t permission = S_stat.st_mode;
   		if(S_ISDIR(permission))
   		{
   			string destination = cmdTokensAbs[cmdTokensAbs.size()-1];
   			char dest[PATH_MAX];
   			strcpy(dest, destination.c_str());
   			char source[PATH_MAX];
   			strcpy(source, cmdTokensAbs[i].c_str());
   			copy_dir(source,dest);

   		}
   		else
   		{
			//appending file name to destination directory to create file/dir
   			string destination = cmdTokensAbs[cmdTokensAbs.size()-1]+"/"+cmdTokens[i];
   			char dest[PATH_MAX];
   			strcpy(dest, destination.c_str());
   			createFile(dest);
   			char source[PATH_MAX];
   			strcpy(source, cmdTokensAbs[i].c_str());
   			singleFileCopy(source,dest);
   		}
				
   	}



   }
   else if(cmdTokens[0].compare("delete_file")==0)
   { 
   	for(int i=1;i<cmdTokens.size();i++)
		{ //each file specified
			struct stat S_stat;
			stat(cmdTokensAbs[i].c_str(),&S_stat);
			mode_t permission = S_stat.st_mode;
			char source[PATH_MAX];
			strcpy(source, cmdTokensAbs[i].c_str());
			//cout << source << " to " << dest;
			if(!S_ISDIR(permission)) 
				delete_file(source);

		}


	}
	else if(cmdTokens[0].compare("delete_dir")==0)
	{ 
		for(int i=1;i<cmdTokens.size();i++)
		{ //each file specified
			struct stat S_stat;
			stat(cmdTokensAbs[i].c_str(),&S_stat);
			mode_t permission = S_stat.st_mode;
			char source[PATH_MAX];
			strcpy(source, cmdTokensAbs[i].c_str());
			//delete if it a directory
			if(S_ISDIR(permission)) 
				delete_dir(source);

		}
	}
	else if(cmdTokens[0].compare("create_file")==0)
	{ 
		for(int i=1;i<cmdTokens.size();i++)
		{
			struct stat S_stat;
			stat(cmdTokensAbs[i].c_str(),&S_stat);
			char source[PATH_MAX];
			strcpy(source, cmdTokensAbs[i].c_str());
			//delete if it a directory
			
			createFile(source);
		}

	}
	else if(cmdTokens[0].compare("create_dir")==0)
	{ 
		for(int i=1;i<cmdTokens.size();i++)
		{
			struct stat S_stat;
			stat(cmdTokensAbs[i].c_str(),&S_stat);
			char source[PATH_MAX];
			strcpy(source, cmdTokensAbs[i].c_str());
			//delete if it a directory
			createDir(source);

		}

	}
	else if(cmdTokens[0].compare("goto")==0)
	{ 
		if(cmdTokens.size() == 2)
		{
			char path[PATH_MAX];
			strcpy(path,cmdTokensAbs[1].c_str());
			struct stat S_stat;
			stat(cmdTokensAbs[1].c_str(),&S_stat);
			mode_t permission = S_stat.st_mode;
			if(S_ISDIR(permission)){ //if the dest if a directory

				//now ensuring that user cannot go out the application root using goto

				int parentLength = cmdTokensAbs[1].find_last_of("/");
				if(parentLength>=strlen(getRoot()-1))
				{ //if the absolule more is less than root length
					gotoFlag=1;
					myLS(path);
					gotoFlag=0;
				}
			}
		}
	}
	else if(cmdTokens[0].compare("snapshot")==0)
	{ 
		if(cmdTokens.size() == 3)
		{
			char folder[PATH_MAX];
			strcpy(folder,cmdTokensAbs[1].c_str());
			char filePath[PATH_MAX];
			strcpy(filePath,cmdTokensAbs[2].c_str());
			snapShotHelper(folder,filePath);
		//	enterCmdMode();
		}
	}
	//move omitted
	else if(cmdTokens[0].compare("move")==0)
	{
		for(int i=1;i<cmdTokens.size()-1;i++)
		{ 
			struct stat S_stat;
			stat(cmdTokensAbs[i].c_str(),&S_stat);
			mode_t permission = S_stat.st_mode;
			if(S_ISDIR(permission)) //source must be a dir
			{
				string destination = cmdTokensAbs[cmdTokensAbs.size()-1];
				char dest[PATH_MAX];
				strcpy(dest, destination.c_str());
				char source[PATH_MAX];
				strcpy(source, cmdTokensAbs[i].c_str());
				stat(destination.c_str(),&S_stat);
				permission = S_stat.st_mode;
				if(S_ISDIR(permission)) //destination also must be a dir
				{
					copy_dir(source,dest);
					delete_dir(source);
				}
			}
			else
			{
					//appending file name to destination directory to create file/dir
				string destination = cmdTokensAbs[cmdTokensAbs.size()-1]+"/"+cmdTokens[i];
				char destt[PATH_MAX];
				strcpy(destt, destination.c_str());
				createFile(destt);
				char source[PATH_MAX];
				strcpy(source, cmdTokensAbs[i].c_str());
				stat(destination.c_str(),&S_stat);
				permission = S_stat.st_mode;
				if(S_ISDIR(permission)) //destination also must be a dir
				{
					singleFileCopy(source,destt);
					delete_file(source);
				}
				
			}
				//delete [] source, dest;
		}
	}
	else if(cmdTokens[0].compare("rename")==0){
		if(cmdTokensAbs.size()==3)
		{
			rename(cmdTokensAbs[1].c_str(),cmdTokensAbs[2].c_str());
		}
	}
	cmdTokens.clear();
	cmdTokensAbs.clear();

	return;
}

void convToAbsolute(){ //updated the vector cmdTokens
	cmdTokensAbs.push_back(cmdTokens[0]); //to match the indices b/w cmdToken and this
	for(int i=1;i<cmdTokens.size();i++){
		string temp = cmdTokens[i];
		if(temp[0]== '/'){

			char* t1=getRoot();
			string root_string=string(t1);

			cmdTokensAbs.push_back(root_string+cmdTokens[i]);
			if(temp.size()>1)
				cmdTokens[i]=temp.substr(1); //removing / so that only file name will be present

		}
		else if(temp[0]=='.' && temp[1]=='.'){
			string currentDir =  dirHistory.list[dirHistory.curIndex];
			int baseLength = currentDir.find_last_of("/");


			cmdTokensAbs.push_back(currentDir.substr(0,baseLength));
			
		}
		else if(temp[0]=='~'){
			char* t1=getRoot();
			string root_string =string(t1);
			cmdTokensAbs.push_back(root_string+cmdTokens[i].substr(1));
			if(temp.size()>1)
				cmdTokens[i]=cmdTokens[i].substr(2); 
			//removing / so that only file name will be present. Will only file name to append


		}
		else if(temp[0]=='.'){
			string currentDir =  dirHistory.list[dirHistory.curIndex];
			cmdTokensAbs.push_back(currentDir+cmdTokens[i].substr(1));
			if(temp.size()>1) // if it is ./file1, /file1 will be place in cmdtoken
				cmdTokens[i]=cmdTokens[i].substr(1); 


		}
		else{ //just file name
			string currentDir = dirHistory.list[dirHistory.curIndex];
			cmdTokensAbs.push_back(currentDir+"/"+cmdTokens[i]);
		}
	}
}




void leftArrowPressed(){ //loading previously visited directory
	setTraverseFlag();
	if(dirHistory.curIndex > 0){
		char path[PATH_MAX];
		dirHistory.curIndex--;
		string temp = dirHistory.list[dirHistory.curIndex];
		strcpy(path,temp.c_str());
		myLS(path);

	}
	resetTraverseFlag();

}

void rightArrowPressed(){
	setTraverseFlag();
	if(dirHistory.curIndex < dirHistory.list.size()-1){
		char path[PATH_MAX];
		dirHistory.curIndex++;
		string temp = dirHistory.list[dirHistory.curIndex];
		strcpy(path,temp.c_str());
		myLS(path);
	}
	resetTraverseFlag();
}

void backSpacePressed(){ //in normal mode, go one directoroy up
	string currentDirectory = dirHistory.list[dirHistory.curIndex]; //current dir
	int baseLength = currentDirectory.find_last_of("/");
	if(baseLength>=strlen(getRoot())) //making sure we don't go out of root dir
	{
		string s_path=currentDirectory.substr(0,baseLength);
		dirHistory.list.push_back(s_path); //Store in history
		dirHistory.curIndex++;
		char path1[PATH_MAX]; //just for conversion purpose (from string to char*)
		strcpy(path1,s_path.c_str());
		myLS(path1);
	}


}

void enterCmdMode(){ //just refreshes screen. Mode and cmdBuffer are set in main
	char path[PATH_MAX];
	string s =  dirHistory.list[dirHistory.curIndex];
	strcpy(path,s.c_str());
	myLS(path);//refresh
}

void exitCmdMode(){//just refreshes screen. Mode is reset in in main
	char path[PATH_MAX];
	string s =  dirHistory.list[dirHistory.curIndex];
	strcpy(path,s.c_str());
	myLS(path);//refresh

}

