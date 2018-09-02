#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
using namespace std;


    set<int> s;
    map<int,int> v;
map<int,int>::iterator it;
int arr[100001];
int marking[100001];
    

void markDone(int marking[],int i){
    if(i%2==0){
        marking[i]=1;
        marking[i+1]=1;
    }
    else{
        marking[i]=1;
        marking[i=1]=1;
    }
}

int allNotMarked(int marking[],int n){
    for(int i=0;i<n;i+=2){
        if(marking[i]==0)
            return 1;
    }
    return 0;
}

int nextElement(int value){
    if(value%2==0){
        it=v.find(value+1);
        int index= it->second ;
        //return index;
        markDone(marking,index);
        if(index%2==0){
            return arr[index+1];
        }
        else{
            it--;
            return arr[index-1];
        }
    }
    else{
        it=v.find(value-1);
        int index=it->second;
        markDone(marking,index);
        //return index;
        if(index%2==0){
            return arr[index+1];
        }
        else{
          
            return arr[index-1];
        }
    }
    cout<< "error";
    return -1;
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    int t,n,i,j,k;
    

    cin >> t;
    for(k=0;k<t;k++){
        int swap=0;
        cin >> n;
        n=2*n;
        int temp;
        for(j=0;j<n;j++){
            cin >>temp;
            arr[j]=temp;
            v.insert(make_pair(temp,j));
        }
        
        while(allNotMarked(marking,n)){
            for(i=0;i<n;i+=2){
                if(!marking[i]){
                    markDone(marking,i);
                    int current = arr[i];
                    cout << "cycle initiated on " << arr[i]<<endl;
                    s.insert(current);
                     while(1){
                         int next = nextElement(current);
                         if(s.find(next)!=s.end()){
                             cout << "cycle size " << s.size() << endl;
                             swap+=s.size()-1;
                             s.clear();
                             break;
                         }
                         else{
                             s.insert(next);
                             current = next;
                         }
                 
                    }
                    
                }
                
               // printf("%d ",marking[i]);
                
                

            }
       // cout << nextElement(1)<<endl;
       }
        
        // s.insert(1);
        // if(s.find(1)==s.end()){
        //     cout << " not present";
        // }
        //cout << nextElement(2)<<endl;
        cout << swap << endl;
        v.clear();
        s.clear();
    }
    
    return 0;
}
