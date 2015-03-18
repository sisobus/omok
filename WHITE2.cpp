#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdlib>
using namespace std;
struct point {
    int y,x;
    point(){}
    point(int _y,int _x):
        y(_y),x(_x){}
};
enum { BLACK,WHITE };
#define HEIGHT 14
#define WIDTH 14
vector<point> stones;
vector<vector<int> > board;
void init() {
    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
        vector<int> v;
        for ( int j = 0 ; j < WIDTH +2 ; j++ ) 
            v.push_back(-1);
        board.push_back(v);
    }

    FILE *fp = fopen("StoneStatus.txt","r");
    int n;
    fscanf(fp,"%d",&n);
    if ( n == 0 ) {
        printf("7 7\n");
        return ;
    }
    for ( int i = 0,turn = BLACK ; i < n ; i++,turn ^= 1 ) {
        int tx,ty;
        fscanf(fp,"%d %d",&tx,&ty);
        stones.push_back(point(ty,tx));
        board[ty][tx] = turn;
    }
    fclose(fp);
}
void solve() {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
    int iam = WHITE;

    for ( int i = 0 ; i < (int)stones.size() ; i++ ) {
        int y = stones[i].y;
        int x = stones[i].x;
        for ( int j = 0 ; j < 8 ; j++ ) {
            int ny = y+dy[j];
            int nx = x+dx[j];
            if ( ny < 0 || nx < 0 || ny >= HEIGHT || nx >= WIDTH ) 
                continue;
            if ( board[ny][nx] == -1 ) {
                board[ny][nx] = iam;
                printf("%d %d\n",nx,ny);
                return ;
            }
        }
    }
}
int main() {
    init();
    solve();
    return 0;
}
