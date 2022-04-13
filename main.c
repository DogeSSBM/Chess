#include "Includes.h"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "en_US.utf8");

    wc board[8][8] = {
        {L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'},
        {L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙'},
        {L'♖',L'♘',L'♗',L'♕',L'♔',L'♗',L'♘',L'♖'}
    };
    GameState state = S_NEUTRAL;
    Color turn = C_WHITE;
    while(state < S_MATE_W){
        clearTerm();

        printTurn(turn);
        printBoard(board);
        while(fgetc(stdin) != '\n');

        turn = nextTurnColor(turn);
    }

    return 0;
}
