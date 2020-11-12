#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <time.h>
#include <iomanip>
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
int m,n,e,R_x,R_y;
int total;
int nowstep;

struct spot{
    short x;
    short y;
    spot(){}
    spot(int a,int b):x(a),y(b){}
};

// main functions
void BFS(int x, int y,int cost, vector <vector<int> > &vis);
void combine(int x, int y, vector<spot> &temp,FILE* tempo); //path + move + charge
void path(int x, int y, vector<spot> &temp); //R to spot
void _path(int &x, int &y, int cost);
void move(int &x, int &y, vector<spot> &temp); //move around
void charge(int &x, int &y, vector<spot> &temp); //go home
void flush(vector<spot> &temp,FILE* tempo); //flush data into temp file
bool bounce(int &x, int &y, vector<spot> &temp); //bounce one step to a smaller place
// minor functions
void debug_i(vector <vector<int> > a); //debugger for int
void debug_b(vector <vector<bool> > a); //debugger for bool
bool outbound(int x, int y); //true if it's outbound

int main(int argc, const char * argv[])
{
    time_t start = time(NULL);
    clock_t start_clock = clock();

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
    cout << R_x << " " << R_y << endl;
    for(int i = 0; i < m; i ++){
        for(int j = 0; j < n; j ++){
            if(!map_clean[i][j]){
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

    time_t stop = time(NULL);
    double duration = (double) difftime(stop,start);
    clock_t stop_clock = clock();
    double duration_clock = ((double) (stop_clock-start_clock))/ CLOCKS_PER_SEC;
    cout << "time: " << duration  << "," << duration_clock << endl;

    cout << "map\n";
    debug_b(map);
    cout << "map_clean\n";
    debug_b(map_clean);
    cout << "vis\n";
    debug_i(vis);

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
        for(int i = 0; i < 4; i ++){
            x0 = temp.x + dx[i];
            y0 = temp.y + dy[i];
            if(x0 >= 0 && y0 >= 0 && x0 < m && y0 < n && vis[x0][y0]== -1 && !clean[x0][y0]){
                vis[x0][y0] = vis[temp.x][temp.y] +1;
                clean[x0][y0] = 1;
                q.push(spot(x0,y0));
            }
        }
    }
    return;
}

void combine(int x, int y, vector<spot> &temp,FILE* tempo)
{
    //cout << "before path: " << x << " " << y << endl;
    path(x,y,temp);
    nowstep = vis[x][y];
    move(x,y,temp);
    //cout << "after move: " << x << " " << y << endl;
    while(bounce(x,y,temp)){
        move(x,y,temp);
    }
    //cout << "before charge: " << x << " " << y << endl;
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
    int t_x[4] = {1,0,-1,0};
    int t_y[4] = {0,1,0,-1};
    int tempx = x;
    int tempy = y;

    for(int i = 0; i < 4; i ++){
        tempx = x+t_x[i];
        tempy = y+t_y[i];
        if(outbound(tempx,tempy)) continue; 
        if(!map_clean[tempx][tempy] && vis[tempx][tempy] == cost){
            x = tempx;
            y = tempy;
            return;
        }
    }
    for(int i = 0; i < 4; i ++){
        tempx = x+t_x[i];
        tempy = y+t_y[i];
        if(outbound(tempx,tempy)) continue; 
        if(vis[tempx][tempy] == cost){
            x = tempx;
            y = tempy;
            return;
        }
    }
}
void move(int &x, int &y, vector<spot> &temp) 
{
    //move around, start from x,y
    int tempx = x;
    int tempy = y;
    while(nowstep < e){
        bool flag = false;
        for(int i = 0; i < 4; i ++){
            tempx = x + dirx[i];
            tempy = y + diry[i];
            if(outbound(tempx,tempy)) continue; 
            if(!map_clean[tempx][tempy] && nowstep + vis[tempx][tempy] < e ){
                x = tempx;
                y = tempy;
                map_clean[x][y] = true;
                temp.push_back(spot(x,y));
                nowstep++;
                flag = true;
                break;
            }
        }
        if(!flag) return;
    }
}
void charge(int &x, int &y, vector<spot> &temp) 
{
    //go home 遞減走回家  
    int cost = vis[x][y];
    int tempx = x;
    int tempy = y;
    bool flag;
    if(cost == 1) return;
    while(cost > 1){
        cost --;
        flag = false;
        for(int i = 0; i < 4; i ++){
            tempx = x + dirx[i];
            tempy = y + diry[i];
            if(outbound(tempx,tempy)) continue;
            if(vis[tempx][tempy] == cost && !map_clean[tempx][tempy]) {
                x = tempx;
                y = tempy;
                map_clean[x][y] = true;
                temp.push_back(spot(x,y));
                flag = true;
                break;
            }
        }
        
        if(flag == true) continue;
        for(int i = 0; i < 4; i ++){
            tempx = x + dirx[i];
            tempy = y + diry[i];
            if(outbound(tempx,tempy)) continue;
            if(vis[tempx][tempy] == cost) {
                x = tempx;
                y = tempy;
                map_clean[x][y] = true;
                temp.push_back(spot(x,y));
                break;
            }
        }
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
void flush(vector<spot> &temp,FILE* tempo)
{
    for(int i = 0; i < temp.size(); i ++) 
        cout << temp[i].x << " " << temp[i].y << endl;
    cout << R_x << " " << R_y << endl;
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