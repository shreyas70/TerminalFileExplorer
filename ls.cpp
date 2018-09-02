
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
int indexx=0; //the file on which cursor is present
vector<string> directoryItems; //list is directories
int scrollingFlag=0; //1 for down,2  for up
//list
int gotoFlag;
int currentViewTerminalLastRow=0;

struct history dirHistory;
void initializeHistory(){
	dirHistory.curIndex=-1;
}


//to handle boundary case. indexx=0 but need to add to history
void addToHistory(char path[]){
	string s_path=string(path);
	dirHistory.list.push_back(s_path);
	dirHistory.curIndex++;
}

void setScrollingFlag(int a){
	scrollingFlag=a; //1 for down, 2 for up
}
int getCurrentViewTerminalLastRow(){
	return currentViewTerminalLastRow;
}

void setTraverseFlag(){
	traverseFlag=1; 
}

void resetTraverseFlag(){
	traverseFlag=0; 
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






int myLS(char path[PATH_MAX]){ //lists given directory

	if(scrollingFlag){
		strcpy(path, dirHistory.list[dirHistory.curIndex].c_str());
	}
	//scrolling mode or cmd => path is current path
	
	DIR *directory; // to open directory

	char fullpath [PATH_MAX];
	directory=opendir(path);
	if(directory==NULL)
		return -1;


	if(!traverseFlag && !getMode() && !scrollingFlag){ //not traversing and not in cmd mode
		//inserting to history
		if(indexx!=0 && indexx!=1){ // if . or .. is not pressed
			string s_path=string(path);
			dirHistory.list.push_back(s_path);
			dirHistory.curIndex++;
		}
		else if(indexx==1){ // .. is pressed
			dirHistory.curIndex--;

		}

	}

	
	struct dirent *S_dirent; //dirent stucture
	struct stat S_stat;

	printf("\e[1;1H\e[2J"); //to clear screen
	directoryItems.clear();




	if( getMode() && gotoFlag){ //if in cmd mode and using goto option
		//inserting to history
		string s_path=string(path);
		dirHistory.list.push_back(s_path);
		dirHistory.curIndex++;
	}


	//cout << "ls called!" << endl;

	

	if(!scrollingFlag){ //what to do if cmd mode? think
		indexx=0;
		currentViewTopIndex=0;

	}

	

	
	mode_t permission;
	int i=0;
	struct winsize S_windowsize;
	ioctl(0, TIOCGWINSZ, &S_windowsize);
	//cout << S_windowsize.ws_col << endl;
	while((S_dirent=readdir(directory))!=NULL){
		//consider adding full path here
		directoryItems.push_back(S_dirent->d_name);
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


			printf(" \t %lld bytes ",S_stat.st_size);


		//time_t t = S_stat.st_mtime;
			time_t S_time=S_stat.st_ctime;
			struct tm lt;
			localtime_r(&S_time, &lt);
			char timbuf[80];
			strftime(timbuf, sizeof(timbuf), "%c", &lt);

			cout << " " << timbuf << " ";

			struct passwd *S_password;
			S_password = getpwuid(S_stat.st_uid);
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
	curListLen=directoryItems.size();
	currentViewTerminalLastRow=S_windowsize.ws_row-2;

	if(!scrollingFlag && !getMode()){ //new directory
		printf("\033[0;0H"); //move cursoe to initial position
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
	

	closedir(directory);
	return 0;
}

//when enter is pressed
void showSelectedDir(){

	//if .. is pressed in root dir nothing should happen

	string s = dirHistory.list[dirHistory.curIndex];

	if(strcmp(s.c_str(),getRoot())==0 && indexx==1)
		return;

	//clearing if there is any unwanted stuff in history
	if(!traverseFlag){ // always true
		if(dirHistory.curIndex < dirHistory.list.size()-1){
			dirHistory.list.erase(dirHistory.list.begin() + dirHistory.curIndex+1 ,dirHistory.list.end());
		}

	}


	char fullpath [PATH_MAX];

	s =  directoryItems[indexx];
	char fileName[PATH_MAX]; //will have the name of the file
	strcpy(fileName, s.c_str()); 


	//current file
	s = dirHistory.list[dirHistory.curIndex];
	snprintf(fullpath, sizeof(fullpath), "%s/%s",s.c_str(), fileName);



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



vector<string> cmdTokens;
vector<string> cmdTokensAbs;



void processCmd(string cmd){
	cout << "processing cmd " << endl;
	char c_cmd[PATH_MAX];
	char *sep = " ";
	char *substring;
	char *last;

	strcpy(c_cmd, cmd.c_str());
	 //string test="sadlfjsadf  sdaflj";
	int j=0;

	for (substring = strtok(c_cmd, sep); substring; substring = strtok(NULL, sep), j++) {
		if(j!=0){ //don't push ':'
			cmdTokens.push_back(string(substring));
		}

	}

	// cout << " dest " << cmdTokens[size-1];



   convToAbsolute(); //updates the vector cmdTokens into absolute paths



   //proccessing

   

	if(cmdTokens[0].compare("copy")==0)
	{
		for(int i=1;i<cmdTokens.size()-1;i++)
		{
			//destination directory
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
				//cout << "copy done" << endl;

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


					//cout << source << " to " << dest;
					
					singleFileCopy(source,dest);
			}
				//delete [] source, dest;
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
			mode_t permission = S_stat.st_mode;
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
			mode_t permission = S_stat.st_mode;
			char source[PATH_MAX];
			strcpy(source, cmdTokensAbs[i].c_str());
			//delete if it a directory
			createDir(source);
					
		}

	}
	else if(cmdTokens[0].compare("goto")==0)
	{ 
		if(cmdTokens.size() == 2){
			char path[PATH_MAX];
			strcpy(path,cmdTokensAbs[1].c_str());
			gotoFlag=1;
			myLS(path);
			gotoFlag=0;
		//	enterCmdMode();
		}
	}
	else if(cmdTokens[0].compare("snapshot")==0)
	{ 
		if(cmdTokens.size() == 3){
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
			if(S_ISDIR(permission))
			{
				string destination = cmdTokensAbs[cmdTokensAbs.size()-1];
				char dest[PATH_MAX];
				strcpy(dest, destination.c_str());
				char source[PATH_MAX];
				strcpy(source, cmdTokensAbs[i].c_str());
				copy_dir(source,dest);
				delete_dir(source);

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
				singleFileCopy(source,destt);
				delete_file(source);
			}
				//delete [] source, dest;
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





void leftArrowPressed(){
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

void enterCmdMode(){
	char path[PATH_MAX];
	//cout << "cmd mode" << endl;

	string s =  dirHistory.list[dirHistory.curIndex];
	strcpy(path,s.c_str());
   // cout << path << endl;
	
	myLS(path);//refresh

}

void exitCmdMode(){
	char path[PATH_MAX];
	//cout << "cmd mode" << endl;

	string s =  dirHistory.list[dirHistory.curIndex];
	strcpy(path,s.c_str());
   // cout << path << endl;
	
	myLS(path);//refresh

}






//will clear the screen, update the things
// int myLS_givenPath(){

// }
