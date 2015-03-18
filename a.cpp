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
void gamePlay() {
    Board board = initBoardStatus(HEIGHT,WIDTH);
    vector<point> stones;
    for ( int seq = 1, turn = 0 ; ; seq++, turn ^= 1 ) {
        render(board);
        printf("현재 %s의 %d번째 턴입니다.\n",turn==WHITE?"WHITE":"BLACK",seq);
        printStonesData(stones);
        printf("\ncode를 %s%d.cpp에 다 작성하셨으면 enter를 눌러주세요.\n",turn==WHITE?"WHITE":"BLACK",seq);
        printf("출력 양식: 좌표값을 x y 의 형식으로 출력한다.\n");
        printf("예제 출력: 2 5\n");
        while ( true ) {
            char ch = getch();
            if ( ch == 'q' ) {
                system("clear");
                printf("\n\t\tGame Quit\n\n");
                exit(-1);
            }
            char filename[1024];
            sprintf(filename,"%s%d",turn==WHITE?"WHITE":"BLACK",seq);
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
                    printf("error] is exists stone in position (%d,%d)\n",ny,nx);
                    continue;
                }
                board[v[1]][v[0]].owner = board[v[1]][v[0]].color = turn;
                stones.push_back(point(v[1],v[0]));
                fclose(fp);
                break;
            }
        }
    }
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
