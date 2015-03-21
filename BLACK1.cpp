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
struct Cell {
    int owner;
    int color, sequence;
    Cell(){}
    Cell(int _owner,int _color,int _sequence):
        owner(_owner),color(_color),sequence(_sequence){}
};
typedef vector<vector<Cell> > Board;
enum { BLACK,WHITE };
#define HEIGHT 14
#define WIDTH 14
vector<point> stones;
Board board;
void init() {
    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
        vector<Cell> v;
        for ( int j = 0 ; j < WIDTH +2 ; j++ ) 
            v.push_back(Cell(-1,-1,-1));
        board.push_back(v);
    }

    FILE *fp = fopen("StoneStatus.txt","r");
    int n;
    fscanf(fp,"%d",&n);
    if ( n == 0 ) {
        printf("7 7\n");
        exit(-1);
    }
    for ( int i = 0,turn = BLACK ; i < n ; i++,turn ^= 1 ) {
        int tx,ty;
        fscanf(fp,"%d %d",&tx,&ty);
        stones.push_back(point(ty,tx));
        board[ty][tx].owner = board[ty][tx].color = turn;
    }
    fclose(fp);
}
bool isOutBound(int y,int x) {
    if ( y <= 0 || x <= 0 || y > HEIGHT || x > WIDTH ) return true;
    return false;
}
int dfs(Board board,int y,int x,int dy,int dx) {
    return 1 + (board[y+dy][x+dx].owner == board[y][x].owner ?
            dfs(board,y+dy,x+dx,dy,dx) : 0);
}
int dir;
int getAdjacentCount(Board board,int y,int x) {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
    int ret = 0;
    for ( int i = 0 ; i < 8 ; i+= 2) {
        int cur = 0;
        int ny = y + dy[i];
        int nx = x + dx[i];
        if ( board[y][x].owner == board[ny][nx].owner )
            cur += dfs(board,ny,nx,dy[i],dx[i]);
        ny = y+dy[i+1];
        nx = x+dx[i+1];
        if ( board[y][x].owner == board[ny][nx].owner )
            cur += dfs(board,ny,nx,dy[i+1],dx[i+1]);
        if ( ret < cur ) {
            ret = cur;
            dir = i;
        }
    }
    return ret + 1;
}
int whereIsThreeStones(Board board,int y,int x) {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
    for ( int i = 0 ; i < 8 ; i += 2 ) {
        int cur = 0;
        int ny = y + dy[i];
        int nx = x + dx[i];
        if ( board[y][x].owner == board[ny][nx].owner ) 
            cur += dfs(board,ny,nx,dy[i],dx[i]);
        ny = y + dy[i+1];
        nx = x + dx[i+1];
        if ( board[y][x].owner == board[ny][nx].owner ) 
            cur += dfs(board,ny,nx,dy[i+1],dx[i+1]);
        if ( cur >= 2 ) return i;
    }
    return -1;
}

int dfs_33(Board board,int y,int x,int dy,int dx,int len) {
    if ( len == 1 ) return (board[y+dy][x+dx].owner == -1);
    return 1 + (board[y+dy][x+dx].owner == board[y][x].owner ?
            dfs_33(board,y+dy,x+dx,dy,dx,len-1):0);
}
bool check_33(Board board,int y,int x) {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
    int tot3 = 0;
    for ( int i = 0 ; i < 8 ; i+=2 ) {
        if ( board[y][x].owner == board[y+dy[i]][x+dx[i]].owner &&
                board[y][x].owner == board[y-dy[i]][x-dx[i]].owner ) 
            tot3 += ( (dfs_33(board,y-dy[i],x-dx[i],-dy[i],-dx[i],1) == 1) &&
                    (dfs_33(board,y+dy[i],x+dx[i],dy[i],dx[i],1) == 1) );
    }
    for ( int i = 0 ; i < 8 ; i++ ) {
        if ( board[y][x].owner == board[y+dy[i]][x+dx[i]].owner ) 
            tot3 += ( ( (dfs_33(board,y+dy[i],x+dx[i],dy[i],dx[i],2) == 2) &&
                        board[y-dy[i]][x-dx[i]].owner == -1) );
    }
    return tot3 > 1;
}
bool if_33(Board board,int y,int x,vector<vector<bool> >& vis) {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1, 0 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1, 0 };
    vis[y][x] = true;
    for ( int i = 0 ; i < 8 ; i++ ) {
        if ( board[y+dy[i]][x+dx[i]].owner == board[y][x].owner ) 
            if ( !vis[y+dy[i]][x+dx[i]] ) 
                if ( if_33(board,y+dy[i],x+dx[i],vis) ) 
                    return true;
    }
    for ( int i = 0 ; i < 9 ; i++ ) 
        if ( board[y+dy[i]][x+dx[i]].owner == board[y][x].owner ) 
            if ( check_33(board,y+dy[i],x+dx[i]) )
                return true;
    return false;
}
#define MAX_DEPTH 4
int max_adj_cnt;
void go(int pos,Board& board,int y,int x,int iam,int turn) {
    if ( pos >= MAX_DEPTH && iam == (turn^1) ) {
        int now = getAdjacentCount(board,y,x);
        if ( max_adj_cnt < now ) 
            max_adj_cnt = now;
        return ;
    }
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };

    for ( int i = 0 ; i < 8 ; i++ ) {
        int ny = y+dy[i];
        int nx = x+dx[i];
        if ( isOutBound(ny,nx) ) continue;
        if ( board[ny][nx].owner != -1 ) continue;
        board[ny][nx].owner = board[ny][nx].color = turn;
        go(pos+1,board,ny,nx,iam,turn^1);
        board[ny][nx].owner = board[ny][nx].color = -1;
    }
}
void printBoardOwner(Board board) {
    puts("");
    for ( int i = 1 ; i <= HEIGHT ; i++ ) {
        for ( int j = 1 ; j <= WIDTH ; j++ ) 
            printf("%d ",board[i][j].owner);
        puts("");
    }
    puts("");
}
void solve() {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
    int iam = BLACK;
    max_adj_cnt = -1;
    point ans;
    for ( int i = 0 ; i < (int)stones.size() ; i++ ) {
        int y = stones[i].y;
        int x = stones[i].x;
        int cur_turn = i%2;
        if ( cur_turn == iam ) {
            int now = getAdjacentCount(board,y,x);
            if ( now == 4 ) {
                int direction = dir;
                int ny = y;
                int nx = x;
                for ( int j = 0 ; j < 5; j++ ) {
                    ny += dy[direction];
                    nx += dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ ) 
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        break;
                    }
                }
                ny = y;
                nx = x;
                for ( int j = 0 ; j < 5 ; j++ ) {
                    ny -= dy[direction];
                    nx -= dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ ) 
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        break;
                    }
                }
            }
        }
        if ( cur_turn != iam ) {
            int now = getAdjacentCount(board,y,x);
            if ( now == 4 ) {
                int direction = dir;
                int ny = y;
                int nx = x;
                for ( int j = 0 ; j < 5 ; j++ ) {
                    ny += dy[direction];
                    nx += dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ ) 
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        break;
                    }
                }
                ny = y;
                nx = x;
                for ( int j = 0 ; j < 5 ; j++ ) {
                    ny -= dy[direction];
                    nx -= dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ )
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        break;
                    }
                }
            }
            else if ( now == 3 ) {
                int direction = dir;
                int ny = y;
                int nx = x;
                bool aleadyDefence = false;
                for ( int j = 0 ; j < 4 ; j++ ) {
                    ny += dy[direction];
                    nx += dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ ) 
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        aleadyDefence = true;
                        break;
                    }
                }
                ny = y;
                nx = x;
                for ( int j = 0 ; j < 4 ; j++ ) {
                    if ( aleadyDefence ) break;
                    ny -= dy[direction];
                    nx -= dx[direction];
                    if ( isOutBound(ny,nx) ) break;
                    vector<vector<bool> > vis;
                    for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                        vector<bool> t;
                        for ( int j = 0 ; j < WIDTH + 2 ; j++ )
                            t.push_back(false);
                        vis.push_back(t);
                    }
                    if ( board[ny][nx].owner == -1 ) {
                        board[ny][nx].owner = board[ny][nx].owner = iam;
                        if ( if_33(board,ny,nx,vis) ) {
                            board[ny][nx].owner = board[ny][nx].owner = -1;
                            break;
                        }
                        printf("%d %d\n",nx,ny);
                        exit(-1);
                    }
                    if ( board[y][x].owner != board[ny][nx].owner ) {
                        aleadyDefence = true;
                        break;
                    }
                }
            }
        }
        for ( int j = 0 ; j < 8 ; j++ ) {
            int ny = y+dy[j];
            int nx = x+dx[j];
            if ( isOutBound(ny,nx) ) 
                continue;
            if ( board[ny][nx].owner != -1 ) continue;
            vector<vector<bool> > vis;
            for ( int i = 0 ; i < HEIGHT + 2 ; i++ ) {
                vector<bool> t;
                for ( int j = 0 ; j < WIDTH + 2 ; j++ ) 
                    t.push_back(false);
                vis.push_back(t);
            }
            board[ny][nx].owner = board[ny][nx].color = iam;
            if ( if_33(board,ny,nx,vis) ) {
                board[ny][nx].owner = board[ny][nx].color = -1;
                continue;
            }
            int last_max_adj_cnt = max_adj_cnt;
            go(0,board,ny,nx,iam,iam^1);
            if ( last_max_adj_cnt < max_adj_cnt ) 
                ans = point(ny,nx);
            board[ny][nx].owner = board[ny][nx].color = -1;
        }
    }
    printf("%d %d\n",ans.x,ans.y);
}
int main() {
    init();
    solve();
    return 0;
}
