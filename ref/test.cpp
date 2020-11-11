#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <time.h>
#include <iomanip>
#include <algorithm>
#include <array>
using namespace std;

typedef pair<int, int> pii;

struct spot{
    short x;
    short y;
    spot(){}
    spot(int a,int b):x(a),y(b){}
};

struct edge{
    spot p;
    int grade;
    edge(){}
    edge(int a,int b, int c):p( spot(a,b)),grade(c) {}
};

bool compare(edge a, edge b)
{
	return a.grade > b.grade;
}

int main()
{
    edge arr[10];
    int temp[10] = {2,5,1,3,4, 9,6,8,100,100};
    for(int i = 0; i < 5; i ++){
        arr[i] = edge(0,i,temp[i]);
    }
    for(int i = 6; i < 9; i ++){
        arr[i] = edge(0,i,temp[i]);
    }
    sort(arr,arr+10,compare);
    for(int i = 0; i < 10; i ++){
        cout << arr[i].p.x << "," << arr[i].p.y << " " << arr[i].grade << endl;
    }

    return 0;
}