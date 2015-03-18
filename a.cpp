#include <cstdio>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
using namespace std;
#pragma warning ( disable : 4996 )
#pragma warning ( disable : 4242 )
#pragma warning ( disable : 4244 )
#pragma warning ( disable : 4204 )
#pragma warning ( disable : 4820 )

#define HEIGHT 14
#define WIDTH 14

#define LEFT_KEY 17
#define UP_KEY 18
#define RIGHT_KEY 19
#define DOWN_KEY 20
#define SPACE_KEY 21
#define ENTER_KEY 22
#define ESC_KEY 23

enum { BLACK,WHITE };
const char *Stone[]={"●","○"};
typedef struct {
    int owner;
    int color, sequence;
} Cell;
typedef vector<vector<Cell> > Board;
struct point {
    int y,x;
    point(){};
    point(int y,int x):
        y (y), x (x){}
};

Board initBoardStatus(int N,int M);
void render(Board board);
void printBoard(Board board);
int getch();
int menu();
void printStonesData(vector<point> stones);
int dfs(Board board,int y,int x,int dy,int dx);
int getWinner(Board board,int y,int x);
int dfs_33(Board board,int y,int x,int dy,int dx,int len);
bool check_33(Board board,int y,int x);
bool if_33(Board board,int y,int x,vector<vector<bool> >& vis);
void printStoneStatus(vector<point>& stones);
void gamePlay() ;

Board initBoardStatus(int N,int M) {
    Board ret;
    for ( int i = 0 ; i < N+2 ; i++ ) {
        vector<Cell> v(M+2);
        ret.push_back(v);
        for ( int j = 0 ; j < M+2 ; j++ ) {
            ret[i][j].owner = -1;
            ret[i][j].color = -1;
            ret[i][j].sequence = 0;
        }
    }
    return ret;
}
void render(Board board) {
    system("clear");
    puts("");
    printBoard(board);    
}
void printBoard(Board board) {
    int N = HEIGHT;
    int M = WIDTH;
    printf("    ");
    for ( int i = 1 ; i < M ; i++ ) 
        printf("%c ",64+i);
    printf("%c ",64+M);
    puts("");
    for ( int i = 1 ; i <= N ; i++ ) {
        printf(" %2d ",i);
        for ( int j = 1 ; j <= M ; j++ ) {
            if ( board[i][j].owner == -1 ) {
                if ( i == 1 ) {
                    if ( j == 1 )
                        printf ( "┌" );
                    else if ( j == M )
                        printf ( "┐" );
                    else
                        printf ( "┬" );
                }
                else if ( i == N ) {
                    if ( j == 1 )
                        printf ( "└" );
                    else if ( j == M )
                        printf ( "┘" );
                    else
                        printf ( "┴" );
                }
                else if ( j == 1 ) {
                    printf ( "├" );
                }
                else if ( j == M ) {
                    printf ( "┤" );
                }
                else {
                    printf ( "┼" );
                }
            } else if ( board[i][j].owner != -1 ) {
                printf("%s",Stone[board[i][j].color]);
            }
            if ( j != M ) 
                printf("─");
        }
        puts("");
    }
}
int getch() {
    struct termios oldt,
                   newt;
    int ch;
    tcgetattr( STDIN_FILENO, &oldt );
    newt = oldt;
    newt.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newt );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
    return ch;
}
enum { PLAY,QUIT };
int menu() {
    system("clear");
    puts("");
    printf("................................................\n");
    printf("................................................\n");
    printf("................................................\n");
    printf("............OO.............mmmmmmmm.............\n");
    printf("..........o....o...........m......m.............\n");
    printf("..........o....o...........m......m.............\n");
    printf("..........o....o...........m......m.............\n");
    printf("............OO.............mmmmmmmm.............\n");
    printf("............OO................mm................\n");
    printf(".......ooooooooooOO........mmmmmmmm.............\n");
    printf("..............................mm................\n");
    printf("...........................mmmmmmmm.............\n");
    printf("..................................m.............\n");
    printf("..................................m.............\n");
    printf("................................................\n");
    printf("......................Created By Sangkeun Kim...\n");
    printf("................................................\n");
    puts("");
    printf("\t\t1. play game? study?\n");
    puts("");
    printf("\t\t2. exit\n");
    puts("");
    char ch = getch();
    return ch-'0'-1;
}
void printStonesData(vector<point> stones) {
    printf("현재 놓여진 돌의 정보\n");
    printf("int n = %d;\n",(int)stones.size());
    printf("int x[]={");
    for ( int i = 0 ; i < (int)stones.size() ; i++ ) 
        printf("%s%d",i==0?"":",",stones[i].x);
    printf("};\n");
    printf("int y[]={");
    for ( int i = 0 ; i < (int)stones.size() ; i++ ) 
        printf("%s%d",i==0?"":",",stones[i].y);
    printf("};\n");
}
int dfs(Board board,int y,int x,int dy,int dx) {
    return 1 + (board[y+dy][x+dx].owner == board[y][x].owner ? 
        dfs(board,y+dy,x+dx,dy,dx) : 0);
}
int getWinner(Board board,int y,int x) {
    static const int dx[] = { 0, 0, 1,-1, 1,-1, 1,-1 },
                 dy[] = { 1,-1, 0, 0, 1,-1,-1, 1 };
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
        if ( cur == 4 ) return board[y][x].owner;
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
void printStoneStatus(vector<point>& stones) {
    FILE *fp = fopen("StoneStatus.txt","w");
    fprintf(fp,"%d\n",(int)stones.size());
    for ( int i = 0 ; i < (int)stones.size() ; i++ )
        fprintf(fp,"%d %d\n",stones[i].x,stones[i].y);
    fclose(fp);
}
void gamePlay() {
    Board board = initBoardStatus(HEIGHT,WIDTH);
    vector<point> stones;
    string lastFileName[2]={"",""};
    bool isNotEnd = true;
    for ( int seq = 1, turn = 0 ; isNotEnd ; seq++, turn ^= 1 ) {
        render(board);
        printf("현재 %s의 %d번째 턴입니다.\n",turn==WHITE?"WHITE":"BLACK",seq);
        printStonesData(stones);
        printf("StoneStatus.txt 파일에 돌의 정보가 저장되어있습니다.\n");
        printf("\ncode를 %s%d.cpp에 다 작성하셨으면 enter를 눌러주세요.\n",turn==WHITE?"WHITE":"BLACK",seq);
        printf("이전에 사용했던 코드(%s)를 사용하시려면 a를 눌러주세요.\n",(int)lastFileName[turn].length()==0?"Nop":lastFileName[turn].c_str());
        printf("출력 양식: 좌표값을 x y 의 형식으로 출력한다.\n");
        printf("예제 출력: 2 5\n");

        while ( true ) {
            char ch = getch();
            if ( ch == 'q' ) {
                system("clear");
                printf("\n\t\tGame Quit\n\n");
                stones.clear();
                printStoneStatus(stones);
                exit(-1);
            }
            char filename[1024];
            if ( ch == 'a' ) {
                if ( (int)lastFileName[turn].length() == 0 ) {
                    printf("error] this turn is first turn\n");
                    continue;
                }
                sprintf(filename,"%s",lastFileName[turn].c_str());
            } else {
                sprintf(filename,"%s%d",turn==WHITE?"WHITE":"BLACK",seq);
            }
            if ( ch != 'a' && ch != 10 ) {
                continue;
            }
            string command = "g++ "+string(filename)+".cpp -o "+string(filename);
            system(command.c_str());
            FILE * fp = fopen(string(string(filename)+".cpp").c_str(),"r");
            if ( fp == NULL ) {
                printf("error] cpp file is not exists!\n");
                continue;
            }
            fclose(fp);
            command = "./"+string(filename)+" > tmp";
            system(command.c_str());

            fp = fopen("tmp","r");
            if ( fp == NULL ) {
                printf("error] compile fail\n");
                continue;
            } else {
                lastFileName[turn] = string(filename);
                char buf[1024]={};
                fscanf(fp,"%[^\n]\n",buf);
                vector<int> v;
                for ( char *p = strtok(buf," \t") ; p ; p = strtok(NULL," \t") ) {
                    int t;
                    sscanf(p,"%d",&t);
                    v.push_back(t);
                }
                if ( (int)v.size() != 2 ) {
                    printf("error] print format is not correct!\n");
                    continue;
                }
                int ny = v[1];
                int nx = v[0];
                if ( ny < 0 || nx < 0 || ny >= HEIGHT || nx >= WIDTH ) {
                    printf("error] your position is outside the board\n");
                    continue;
                }
                if ( board[ny][nx].owner != -1 ) {
                    printf("error] is exists stone in position (%d,%d)\n",nx,ny);
                    continue;
                }
                vector<vector<bool> > vis;
                for ( int i = 0 ; i < HEIGHT+2 ; i++ ) {
                    vector<bool> t;
                    for ( int j = 0 ; j < WIDTH+2 ; j++ ) 
                        t.push_back(false);
                    vis.push_back(t);
                }
                board[ny][nx].owner = board[ny][nx].color = turn;
                board[ny][nx].sequence = seq;
                if ( if_33(board,ny,nx,vis) ) {
                    printf("error] 3*3 \n");
                    board[ny][nx].owner = board[ny][nx].color = -1;
                    board[ny][nx].sequence = 0;
                    continue;
                }
                stones.push_back(point(v[1],v[0]));
                printStoneStatus(stones);

                if ( getWinner(board,ny,nx) == turn ) {
                    render(board);
                    printf("%s is winner!\n",turn==WHITE?"WHITE":"BLACK");
                    isNotEnd = false;
                    ch = getch();
                    break;
                }
                fclose(fp);
                break;
            }
        }
    }
    stones.clear();
    printStoneStatus(stones);
}
int main() {
    int exit = 0;

    while ( ~exit ) {
        switch ( menu() ) {
            case PLAY:gamePlay(); break;
            case QUIT:exit = -1; break;
            default: break;
        }
    }
    system("clear");
    return 0;
}
