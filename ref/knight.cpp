#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>
// 這個似乎可以運作 ><
#define mp make_pair
#define fi first
#define se second
#define siz 4
using namespace std;
const int MAX = 100, INF = 0x3f3f3f3f;
//const int dx[8] = {-1, -2, -2, -1, 1, 2, 2, 1}, dy[8] = {-2, -1, 1, 2, 2, 1, -1, -2};
//int dx[siz]={-1,0,1,0};
//int dy[siz]={0,1,0,-1};
int dx[siz]={0,0,-1,1};
int dy[siz]={-1,1,0,0};

typedef pair<int, int> pii;
bool vis[MAX][MAX], ok; 
vector<pii> path;  
int m,n, x, y;
int parr[MAX][MAX]; 
int wall;
int electric;
void get_input();

void dfs(){
	if(path.size() == m * n -wall){
		ok = true; 
        for(int i = 0; i < path.size(); ++i) 
            parr[path[i].fi][path[i].se] = i + 1;
		return;
	}
	int x = path.back().fi, y = path.back().se; 
	pii dir[siz]; 
    memset(dir, 0, sizeof(dir)); 
	//將x, y周圍的所有結點按可選方案數遞增依次加入dir中 
	for(int i = 0; i < siz; ++i){
		dir[i].se = i;
		int a = x + dx[i], b = y + dy[i];
		if(a >= 1 && a <= m && b >= 1 && b <= n && !vis[a][b]){
			for(int j = 0; j < siz; ++j){
				int c = a + dx[j], d = b + dy[j];
				if(c >= 1 && c <= m && d >= 1 && d <= n && !vis[c][d]) 
                    ++dir[i].fi;
			}
		}
	}
	sort(dir, dir + siz); // what does this sort do 
	for(int i = 0; i < siz; ++i){
		if(!dir[i].fi && path.size() != m * n - wall -1) continue;
		int a = x + dx[dir[i].se], b = y + dy[dir[i].se];
		if(a >= 1 && a <= m && b >= 1 && b <= n && !vis[a][b]){
			path.push_back(mp(a, b)), vis[a][b] = true, dfs(); if(ok) return;
			path.pop_back(), vis[a][b] = false;
		}	
	}
}
int main(){
	int x, y; 
    n = 8, cin >> x >> y, vis[x][y] = true;
	m = n;
    vis[1][8] = true;
    wall ++; 
	
	//get_input();
    path.push_back(mp(x, y)), dfs();
	for(int i = 1; i <= m; i++, cout << endl)
		for(int j = 1; j <= n; j++) 
            cout.width(4), cout << parr[i][j];
	return 0;
}

void get_input()
{
	cin >> m >> n >> electric;
	for(int i = 1; i <= m; i ++){
        for(int j = 1; j <= n; j ++){
            char temp;
            cin >> temp;
            if(temp == '1') {
				vis[i][j] = 1;
				wall ++;
			}
            else if(temp == '0') vis[i][j] = 0;
            else if(temp == 'R') {
                x = i;
                y = j;
                vis[i][j] = 0;
            }
        }
    }
}