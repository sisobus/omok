#include <cstdio>
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

enum { WHITE,BLACK };
const char *Stone[]={"○","●"};
typedef struct {
    int owner;
    int color, sequence;
} Cell;
typedef vector<vector<Cell> > Board;
struct point {
    int y,x;
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
    printf("1. play game? study?\n");
    puts("");
    printf("2. exit\n");
    puts("");
    char ch = getch();
    return ch-'0'-1;
}
void gamePlay() {
    Board board = initBoardStatus(HEIGHT,WIDTH);
    while ( true ) {
        render(board);
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
