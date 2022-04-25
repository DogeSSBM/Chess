#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Board board = {
        {L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'},
        {L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '},
        {L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙'},
        {L'♖',L'♘',L'♗',L'♕',L'♔',L'♗',L'♘',L'♖'}
    };

    Color turn = C_WHITE;
    Turn *game = NULL;
    do{
        const Move move = getColorsMove(board, game, turn);
        game = applyMove(board, game, move);
        turn = colorInv(turn);
    }while(lastTurn(game)->state != S_MATE);

    return 0;
}
