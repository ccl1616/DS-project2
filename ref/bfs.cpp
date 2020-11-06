#include<iostream>
#include<cstring>
#include<queue>
using namespace std;
#define INF 0x3f3f3f3f
typedef pair<int,int> P;
const int MAX = 100;
char maze[MAX][MAX+1];
int m,n;
int sx,sy;//起點坐標
int gx,gy;//終點坐標 

int dx[4]={1,0,-1,0},dy[4]={0,1,0,-1};
int d[MAX][MAX+1];

int bfs(){
    queue<P> que; 
    //尋找起點和終點坐標
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            if(maze[i][j]==‘S‘){
                sx=i;sy=j;
            }
            if(maze[i][j]==‘G‘){
                gx=i;gy=j;
            }
        }
    } 
    
    memset(d,INF,sizeof(d));
    //將初始狀態（起點）加入隊列  
    que.push(P(sx,sy));
    d[sx][sy]=0; 
    while(que.size()){
        //取出隊頭 
        P p = que.front();
        que.pop();
        int nx=p.first,ny=p.second;
        if(nx==gx&&ny==gy){
            break;
        }
        for(int i=0;i<4;i++){
            //移動後的位置記為（nx，ny） 
            nx=p.first + dx[i];ny=p.second + dy[i];
            if(nx>=0&&ny>=0&&nx<m&&ny<n&&d[nx][ny]==INF&&maze[nx][ny]!=‘#‘){
                d[nx][ny]=d[p.first][p.second]+1;
                que.push(P(nx,ny));
            }
        }
    }
    return d[gx][gy];
}

int main(){
    cin>>m>>n;
    for(int i=0;i<m;i++){
        cin>>maze[i];
    }
    cout<<bfs()<<endl;
//    cout<<sx<<" "<<sy<<endl;
//    cout<<gx<<" "<<gy<<endl;
    return 0;
} 