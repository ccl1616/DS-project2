// https://www.itread01.com/content/1544340542.html
#include<iostream>
#include<algorithm>
#include<stack>
using namespace std;
typedef pair<int,int> p;
const int N=500;
int visited[N][N],map[N][N];
int dfs(int x,int y);
int dir[4][2] = { {0,1},{1,0},{0,-1},{-1,0} };

int n,m;
stack<p>s;
int main(){
	cin>>n>>m;
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			cin>>map[i][j];
	for(int i=0;i<n;i++)
		for(int j=0;j<m;j++)
			visited[i][j] = 0;  
    if(dfs(0,0)){
    	 s.push(make_pair(0,0));
    }
    while(!s.empty()){
    	p t = s.top();
    	cout<<"("<<t.first<<","<<t.second<<")"<<endl;
    	s.pop();
    }
    return 0;
}
int dfs(int x,int y){
	visited[x][y] = 1;
    if( x != 0 && y != 0){
        if(x == 0 || x==n-1 || y == 0 || y==m-1) {
            s.push(make_pair(x,y));
            return 1;
        }
    }
	for(int i=0;i<4;i++){
		int x1 = x+ dir[i][0];
		int y1 = y+ dir[i][1];
		if(x1<n&&y1<m&&x1>=0&&y1>=0&&visited[x1][y1]==0&&map[x1][y1]==0){
			visited[x1][y1] = 1;
			if(dfs(x1,y1)){
				//s.push(make_pair(x1,y1));
				return 1;
			}			
		}
	}
return 0;
}