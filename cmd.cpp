
#include "cmd.h"

//This functions needs the destination file to exit, so create it,
//append to full path with file name then send. 
int singleFileCopy ( char * source,  char* destination) {
	std::ifstream  from(source, std::ios::binary);
	std::ofstream  to(destination, std::ios::binary);

	struct stat S_stat;
	to << from.rdbuf();
	//current user will own the file

	stat(source, &S_stat); //fetching metadata about file

	//setting permission
	// the last 4 octal digits of mode correspond to permissions
	chmod(destination, S_stat.st_mode & 07777);
	chown(destination, S_stat.st_uid, S_stat.st_gid);
	return 1;
}

int createFile(char * path1){
	return open(path1,O_CREAT,0644); //passing default permissions
	//current user will own the file
}

int createDir(char *path){
	return mkdir(path,0775);

}

int delete_file(char *path){ 
	return unlink(path); ;
	
}

int delete_empty_dir(char *path){ //will delete empty directory
	return rmdir(path);
}

//pass absolute paths
int moveFile(char * path1, char* path2){
	singleFileCopy (path1, path2);
	delete_file(path1); 
	return 1;
}

int moveDir(char * path1, char* path2){
	copy_dir(path1,path2);
	delete_dir(path1);
	return 1;
}

//recursive call to create snapshot. Uses just opendir, not NFTW
//uses a queue to store all directories encountered
int snapShot(char * path, int outputFD){
	DIR *directory; // to open directory
	directory=opendir(path);
	if(directory==NULL){
		return -1;
	}

	struct dirent *S_dirent; //dirent stucture
	struct stat S_stat;
	mode_t permission; //to check if it a dir

	queue<string> q;

	string s=string(path);
	write(outputFD,"\n",sizeof("\n"));

	//subtracting application root length before printing any path
	int rootLength = strlen(getRoot());
	string truncated = string(path);
	cout << rootLength << ' ';

	char temp[PATH_MAX];
	strcpy(temp,truncated.substr(rootLength).c_str());
	write(outputFD, temp, strlen(temp));
	write(outputFD,":",strlen(":"));
	write(outputFD,"\n",strlen("\n"));


	 while((S_dirent=readdir(directory))!=NULL){
	 	if(S_dirent->d_name[0]=='.')
	 	{
	 		//skipping hidden files
	 		continue;
	 	}
	 	write(outputFD,S_dirent->d_name,strlen(S_dirent->d_name));
	 	write(outputFD,"\n",strlen("\n"));
	 	
	 	string fullpath=s+"/"+S_dirent->d_name;
	 	stat(fullpath.c_str(),&S_stat);
	 	permission = S_stat.st_mode;
	 	if(S_ISDIR(permission)){
	 		q.push(fullpath);
	 	}
	 	
	}

	while(!q.empty()){
		s = q.front();
		q.pop();
		char temp[s.size()];
		strcpy(temp,s.c_str());
		//recursive call on each directory
		snapShot(temp, outputFD);
	}
	write(outputFD,"\n",strlen("\n"));
	return 1;
}


//creates file and calls the recursive snapshot function
int snapShotHelper(char * path, char* filename){

	createFile(filename);
	int output= open(filename,O_WRONLY); //this file has to be open throughout the recursive call
	snapShot(path,output);
	
	return 1;
}


/*
This function is passed to post order NFTW call. 
Post order because all the files in the directory has to be delted before the directory is deleted.
*/
static int nftw_delete(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf)
{
	char path[PATH_MAX];
	strcpy(path,fpath);
	mode_t permission = sb->st_mode;
	 	if(S_ISDIR(permission))
	 	{
	 		printf("Deleted dir %s\n",path);
	 		delete_empty_dir(path);
	 	}
	 	else
	 	{
	 		printf("Deleted file %s\n",path);
	 		delete_file(path);
	 	}
    return 0;           
}


int delete_dir(char * path){
	 if (nftw(path, nftw_delete, 20, FTW_DEPTH ) == -1) {
        cout << ("nftw error");
        return -1;
    }
    return 1;
}

char path_buf[PATH_MAX]; //used for conversion from string to char * or vice versa
int baseLength;

/*
This function is passed to Pre order NFTW call. 
Pre order because the directory has to be created before the copying the files
*/

static int nftw_copy(const char *fpath, const struct stat *sb,int tflag, struct FTW *ftwbuf)
{
	char path[PATH_MAX];
	strcpy(path,fpath);
	string destination=string(path_buf);
	string source=string(fpath);

	string temp = destination + source.substr(baseLength);


	mode_t permission = sb->st_mode;
	 	if(S_ISDIR(permission)){
	 		//creating directory
	 	
	 		char dest[PATH_MAX];
	 		strcpy(dest,temp.c_str());
	 		createDir(dest);
	 	}else
	 	{
	 		//creating file and copying content
	 		printf("created file %s\n",temp.c_str());
	 		char dest[PATH_MAX];
	 		strcpy(dest,temp.c_str());
	 		singleFileCopy(path,dest);
	 		
	 	}
    return 0;           
}



int copy_dir(char * sourceDir, char * destination){
	// Note - destination shouldn't have a slash at the end

	//sourceDir - is a path which should end with a directory. This condition is checked in processCmd function

	string s=string(sourceDir);
	baseLength = s.find_last_of("/");


	strcpy(path_buf,destination); //copying path for the other function to be able to copy

	 if (nftw(sourceDir, nftw_copy, 20, 0 ) == -1) {
        perror("nftw");
        return -1;
    }
    return 1;
}



//Commented block of code below can be used to test the functions
/*

int main()
{

	moveDir();

	singleFileCopy("","");

	createFile("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/created.txt");

	singleFileCopy("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/p2.txt","/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/WorkingDir/Dir1");

	delete_file("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/CreatedDir");

	moveFile("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/p4.txt","/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/CreatedDir/p4.txt");

	snapShotHelper("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer", "snapshot.txt");

	delete_dir(" ");

	copy_dir("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/Dir1/Dir2","/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/Dir1/Dir3");

	createDir("/Users/shreyas/Downloads/VB/GitHub/TerminalFileExplorer/CreatedDir");

	return 1;

}
*/



