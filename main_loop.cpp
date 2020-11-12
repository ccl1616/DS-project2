#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <time.h>
#include <iomanip>
#include <algorithm>
using namespace std;
// my code
vector <vector<bool> > map; //for input
vector <vector<bool> > map_clean; //for now situation
vector <vector<int> > vis;
// 上下左右
int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1};
// 右左下上
int dirx[4] = {0,0,1,-1};
int diry[4] = {1,-1,0,0};
// 下右上左
int t_x[4] = {1,0,-1,0};
int t_y[4] = {0,1,0,-1};
int m,n,e,R_x,R_y;
int total;
int nowstep;

struct spot{
    short x;
    short y;
    spot(){}
    spot(int a,int b):x(a),y(b){}
};
struct spot_{
    short x;
    short y;
    int cost;
    spot_(){}
    spot_(int a,int b):x(a),y(b),cost(0){}
    spot_(int a,int b,int c):x(a),y(b),cost(c){}
};
struct myCompare { 
    bool operator()(spot_ const& p1, spot_ const& p2) 
    { 
        // return "true" if "p1" is ordered  
        // before "p2", for example: 
        return p1.cost < p2.cost; 
        //return 0; 
    } 
}; 
priority_queue<spot_, vector<spot_>, myCompare> PQ; 
//queue<spot_, vector<spot_> > PQ; 

// main functions
void BFS(int x, int y,int cost, vector <vector<int> > &vis);
void combine(int x, int y, vector<spot> &temp,FILE* tempo); //path + move + charge
void path(int x, int y, vector<spot> &temp); //R to spot
void _path(int &x, int &y, int cost);
void move(int &x, int &y, vector<spot> &temp); //move around
void charge(int &x, int &y, vector<spot> &temp); //go home
void flush(vector<spot> &temp,FILE *tempo); //flush data into temp file
bool bounce(int &x, int &y, vector<spot> &temp); //bounce one step to a smaller place

// minor functions
void debug_i(vector <vector<int> > a); //debugger for int
void debug_b(vector <vector<bool> > a); //debugger for bool
bool outbound(int x, int y); //true if it's outbound
bool shape_u(int x, int y);

time_t start;
clock_t start_clock;
time_t stop;
double duration;
clock_t stop_clock;
double duration_clock;

int main(int argc, const char * argv[])
{
    if(argc != 2) exit(1);
    start = time(NULL);
    start_clock = clock();

    // initialize and input
    FILE* input = freopen(argv[1],"r",stdin);
    scanf("%d %d %d",&m,&n,&e);
    if(m > 1000 || n > 1000){
        cout << "invalid map size\n";
        return 0;
    }
    vis.resize(m);
    map.resize(m);
    map_clean.resize(m);
    for(int i = 0; i < m; i ++){
        vis[i].resize(n);
        map[i].resize(n);
        map_clean[i].resize(n);
    }
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            char temp;
            cin >> temp;
            if(temp == 'R') {
                R_x = i;
                R_y = j;
                map[i][j] = true;
            }
            else if(temp == '1') map[i][j] = true;
            else if(temp != '0') {
                cout << "invalid test case with character: " << temp << endl;
                return 0;
            }
            else map[i][j] = false;

            map_clean[i][j] = map[i][j];
        }
    }
    fclose(input);
    
    // generate path
    FILE *tempo = freopen("temp.txt","w",stdout);
    vector<spot> temp;
    BFS(R_x,R_y,0,vis);
    stop_clock = clock();
    double duration_temp = ((double) (stop_clock-start_clock))/ CLOCKS_PER_SEC;

    cout << R_x << " " << R_y << endl;
    
    while(!PQ.empty()){
        spot_ p = PQ.top();
        PQ.pop();
        if(!map_clean[p.x][p.y]){
            combine(p.x,p.y,temp,tempo);
        }
    }

    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            while(!map_clean[i][j]){
                combine(i,j,temp,tempo);
            }
        }
    }

    // output result
    FILE* output = freopen("final.path","w",stdout);
    tempo = freopen("temp.txt","r",stdin);
    cout << "total: " << total << endl;
    /*
    int a = 0;
    int b = 0; 
    for(int i = 0; i < total; i ++){
        fscanf(tempo,"%d %d",&a,&b);
        fprintf(output,"%d %d\n",a,b);
    }*/

    stop = time(NULL);
    duration = (double) difftime(stop,start);
    stop_clock = clock();
    duration_clock = ((double) (stop_clock-start_clock))/ CLOCKS_PER_SEC;
    cout << "time: " << duration  << "," << duration_clock << endl;
    
    while(!PQ.empty() ){
        spot_ p = PQ.top();
        PQ.pop();
        cout << p.x << "," << p.y << " " << p.cost << endl;
    }
    
    fclose(tempo);
    fclose(output);
    return 0;
}
void BFS(int x, int y,int cost, vector <vector<int> > &vis)
{
    // initialize
    bool clean[m][n];
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            vis[i][j] = -1;
            clean[i][j] = map[i][j];
        }
    }
    // x,y for starting pos
    queue<spot> q;
    q.push(spot(x,y));
    vis[x][y] = 0; //此點為起點

    while(!q.empty()){

        spot temp = q.front();
        q.pop();
        int x0 = temp.x;
        int y0 = temp.y;
        if(vis[x0][y0] == e/2){
            PQ.push(spot_(x0,y0,vis[x0][y0]) );
            continue;
        }

        if(x0-1>=0 && !clean[x0-1][y0]){
            vis[x0-1][y0] = vis[x0][y0] +1;
            clean[x0-1][y0] = 1;
            q.push(spot(x0-1,y0));
        }
        if(x0+1<m && !clean[x0+1][y0]){
            vis[x0+1][y0] = vis[x0][y0] +1;
            clean[x0+1][y0] = 1;
            q.push(spot(x0+1,y0));
        }
        if(y0+1<n && !clean[x0][y0+1]){
            vis[x0][y0+1] = vis[x0][y0] +1;
            clean[x0][y0+1] = 1;
            q.push(spot(x0,y0+1));
        }
        if(y0-1>=0 && !clean[x0][y0-1]){
            vis[x0][y0-1] = vis[x0][y0] +1;
            clean[x0][y0-1] = 1;
            q.push(spot(x0,y0-1));
        }
        if(shape_u(x0,y0) || vis[x0][y0]==e/2 ){
            PQ.push(spot_(x0,y0,vis[x0][y0]) );
        }
    }
    return;
}

void combine(int x, int y, vector<spot> &temp,FILE* tempo)
{
    path(x,y,temp);
    nowstep = vis[x][y];
    move(x,y,temp);
    
    while(bounce(x,y,temp)){
        move(x,y,temp);
    }
    charge(x,y,temp);
    flush(temp,tempo);
}

void path(int x, int y, vector<spot> &temp){
    // generate path from R to (x,y)
    stack<spot> s;
    int cost = vis[x][y];
    map_clean[x][y] = true;
    s.push(spot(x,y));
    while(cost > 1){
        cost--;
        _path(x,y,cost);
        //_path_wall(x,y,cost);
        s.push(spot(x,y));
        map_clean[x][y] = true;
    }
    while(!s.empty()){
        temp.push_back(s.top());
        s.pop();
    }
} 
void _path(int &x, int &y, int cost )
{
    // 下右上左
    if( x+1<m && vis[x+1][y]==cost && !map_clean[x+1][y] ) x += 1;
    else if( y+1<n && vis[x][y+1]==cost && !map_clean[x][y+1] ) y += 1;
    else if( x-1>=0 && vis[x-1][y]==cost && !map_clean[x-1][y] ) x -= 1;
    else if( y-1>=0 && vis[x][y-1]==cost && !map_clean[x][y-1] ) y -=1;
    else if( x+1<m && vis[x+1][y]==cost ) x += 1;
    else if( y+1<n && vis[x][y+1]==cost  ) y += 1;
    else if( x-1>=0 && vis[x-1][y]==cost  ) x -= 1;
    else if( y-1>=0 && vis[x][y-1]==cost  ) y -=1;
    else {
        cout << "impossible\n";
        exit(1);
    }
    
}
void move(int &x, int &y, vector<spot> &temp) 
{
    //move around, start from x,y
    // 右左下上
    while(nowstep < e){
        if(y+1<n && nowstep+vis[x][y+1]<e && vis[x][y+1] != -1 && !map_clean[x][y+1] ) y ++;
        else if(y-1>=0 && nowstep+vis[x][y-1]<e && vis[x][y-1] != -1 && !map_clean[x][y-1] ) y --;
        else if(x+1<m && nowstep+vis[x+1][y]<e  && vis[x+1][y] != -1 && !map_clean[x+1][y] ) x ++;
        else if(x-1>=0 && nowstep+vis[x-1][y]<e && vis[x-1][y] != -1 && !map_clean[x-1][y] ) x --;
        else return;
        map_clean[x][y] = true;
        temp.push_back(spot(x,y));
        nowstep++;
    }
}
void charge(int &x, int &y, vector<spot> &temp) 
{
    //go home 遞減走回家  
    x = temp.back().x;
    y = temp.back().y;
    int cost = vis[x][y];
    if(cost == 1) return;
    while(cost > 1){
        cost --;
        // 右左下上
        if(y+1<n && vis[x][y+1]==cost && !map_clean[x][y+1] ) y ++;
        else if(y-1>=0 && vis[x][y-1]==cost && !map_clean[x][y-1] ) y --;
        else if(x+1<m && vis[x+1][y]==cost && !map_clean[x+1][y] ) x ++;
        else if(x-1>=0 && vis[x-1][y]==cost && !map_clean[x-1][y] ) x --;

        else if(y+1<n && vis[x][y+1]==cost ) y ++;
        else if(y-1>=0 && vis[x][y-1]==cost ) y --;
        else if(x+1<m && vis[x+1][y]==cost ) x ++;
        else if(x-1>=0 && vis[x-1][y]==cost ) x --;
        else{
            cout << "impossible\n";
            exit(1);
        }
        map_clean[x][y] = true;
        temp.push_back(spot(x,y));
    }
}

bool bounce(int &x, int &y, vector<spot> &temp)
{
    // bounce return false if previous step is not vis-1, or if bounce will cause 電力不足以回家
    int id = temp.size()-2;
    int x_ = temp[id].x;
    int y_ = temp[id].y;
    if(vis[x_][y_] == vis[x][y]-1){
        if(nowstep+1+vis[x_][y_] > e) {
            cout << nowstep << " " << x << "," << y << " to " << x_ << "," << y_ << endl;
            cout << vis[x_][y_] << " " << e << endl;
            return false;
        }
        //cout << "bounce from: " << x << "," << y << " to " << x_ << "," << y_ << endl;
        x = x_, y = y_;
        temp.push_back(spot(x,y));
        nowstep++;
        return true;
    }
    //cout << "bounce false from: " << x << "," << y << " to " << x_ << "," << y_ << endl;
    return false;
}

// minor functions
void flush(vector<spot> &temp,FILE *tempo)
{
    for(int i = 0; i < temp.size(); i ++) 
        fprintf(tempo,"%d %d\n",temp[i].x,temp[i].y);
    fprintf(tempo,"%d %d\n",R_x,R_y);
    total += temp.size()+1;
    temp.clear();
}
bool outbound(int x, int y)
{
    if(x < 0 || y < 0 || x >= m || y >= n ) return true;
    else return false;
}
void debug_i(vector <vector<int> > a)
{
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            if(a[i][j] == -1) cout << setw(3) << "-" << " ";
            else cout << setw(3) << a[i][j] << " ";
        }
        cout << endl;
    }
}
void debug_b(vector <vector<bool> > a)
{
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            cout << setw(3) << a[i][j] << " ";
        }
        cout << endl;
    }
}
bool shape_u(int x, int y)
{
    int counter = 0;
    int dir = -1;
    for(int i = 0; i < 4; i ++){
        if(counter > 1) return false;
        if(outbound(x+dx[i],y+dy[i]) ) continue;
        if(!map[x+dx[i]][y+dy[i]] ){
            counter++;
            dir = i;
        }
    }
    if(counter != 1) return false;
    return true;
}