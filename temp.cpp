

#include <sys/ioctl.h>
#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

struct history
{
	vector<string> list;
	int curIndex;
	
};

int main (void)
{
	struct history hq;
	
		
	cout << hq.curIndex <<endl;
	hq.list.push_back("1");
	hq.list.push_back("2");

	vector<string>::iterator it;

	for(auto i=hq.list.begin();i!=hq.list.end();i++){
		cout << *i << endl;
	}

    return 0;
}