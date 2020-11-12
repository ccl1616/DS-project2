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
    } 
}; 



int main()
{
    priority_queue<spot_, vector<spot_>, myCompare> Q; 
  
    // When we use priority_queue with  structure 
    // then we need this kind of syntax where 
    // CompareAge is the functor or comparison function 
    vector<spot_> arr;
    for(int i = 0; i < 10; i ++)
        Q.push(spot_(i,i,10-i) );

    while (!Q.empty()) { 
        spot_ p = Q.top(); 
        Q.pop(); 
        cout << p.x << "," << p.y << " " << p.cost << endl;
    } 
    return 0; 
}