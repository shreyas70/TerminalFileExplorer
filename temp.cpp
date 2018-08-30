#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


int a[3][3]={0};
int b[3][3];
int add[8][6]={
    {0,0,0,1,0,2},
    {1,0,1,1,1,2},
    {2,0,2,1,2,2},
    {0,0,1,0,2,0},
    {0,1,1,1,2,1},
    {0,2,1,2,2,2},
    {0,0,1,1,2,2},
    {0,2,1,1,2,0}
};

int min_diff=10000000;


vector<int> update(vector<int> req, int a[][3]){
    for(int i=0;i<8;i++){
        int sum=0;
        for(int j=0;j<6;j+=2){
            sum+=a[add[i][j]][add[i][j+1]];
        }
        req[i]=15-sum;
    }
    return req;
}

vector<int> update(vector<int> req, int a[][3], int i){
   // for(int i=0;i<8;i++){
        int sum=0;
        for(int j=0;j<6;j+=2){
            sum+=a[add[i][j]][add[i][j+1]];
        }
        req[i]=15-sum;
   // }
    //cout << req[i] << endl;
    return req;
}


int mat_diff(int a[][3],int b[][3]){
    int i,j,sum=0;
    for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            sum+=abs(a[i][j]-b[i][j]);
        }
    }
    return sum;
}


void MM_helper(int a[][3],vector<int> req,int n){
    if(n==8){
        //print
        req=update(req,a);
        int flag=0;
        for(auto it=req.begin();it!=req.end();it++){
        if(*it!=0){
            flag=1;
            break;
        } 
        }
        if(flag==0){
            int temp = mat_diff(a,b);
            if(min_diff>temp)
                min_diff=temp;
        }
        
        return;
    }
    int temp=req[n];
    int i;
    for(i=0;i<6;i+=2){
        a[add[n][i]][add[n][i+1]] += req[n];
        req=update(req,a);
        MM_helper(a,req,n+1);
        a[add[n][i]][add[n][i+1]] -=temp;
        req=update(req,a);
        
    }
    
}

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    //ios_base::sync_with_stdio(false);
    int x,y,c,d,e,f,g,h,i;
    
    int j;
    vector<int> req(8);
    
      for(i=0;i<3;i++){
        for(j=0;j<3;j++){
            cin >> a[i][j];
            b[i][j]=a[i][j];
           // cout << a[i][j] << " ";
        }
    }
    
    
    int n=0;
    
    
    for(x=1;x<10;x++){
        a[0][0]=5;
        for(y=1;y<10;y++){
            a[0][1]=5;
            for(c=1;c<10;c++){
                a[0][2]=5;
                req=update(req,a,0);
                if(req[0]==0){
                   // cout << "row 1 passed " << endl;
                for(d=1;d<10;d++){
                    a[1][0]=d;
                    for(e=1;e<10;e++){
                        a[1][1]=e;
                        for(f=1;f<10;f++){
                            a[1][2]=f;
                            req=update(req,a,1);
                            if(req[1]==0){
                            for(g=1;g<10;g++){
                                a[2][0]=g;
                                for(h=1;h<10;h++){
                                    a[2][1]=h;
                                    for(i=1;i<10;i++){
                                        a[2][2]=i;
                                         req=update(req,a);
                                            int flag=0;
                                            for(auto it=req.begin();it!=req.end();it++){
                                            if(*it!=0){
                                                flag=1;
                                                break;
                                            } 
                                            }
                                            if(flag==0){
                                                int temp = mat_diff(a,b);
                                                if(min_diff>temp)
                                                    min_diff=temp;
                                            }
                                    }
                                }
                            }
                        }
                        }
                    }
                }
                }
       }
    }
}
    
    
    
  
    
//     for(k=0;k<10;k++){
//                 a[i][j]=k;
//             }
    
    
//     for(i=0;i<3;i++){
//         for(j=0;j<3;j++){
               
//         }
//     }
    
    
    
    
//     req = update(req,a);
    
//     MM_helper(a,req,0);
    
    cout << min_diff <<endl;
    
    return 0;
}
