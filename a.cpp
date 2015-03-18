#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <ncurses.h>
#include <cstdlib>
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
int main() {
    Board board = initBoardStatus(HEIGHT,WIDTH);
    board[1][1].owner = board[1][1].color = WHITE;
    board[2][1].owner = board[2][1].color = BLACK;

    render(board);

    return 0;
}
