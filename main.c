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

    GameState state = S_NEUTRAL;
    Color turn = C_WHITE;
    Turn *game = NULL;
    do{
        const Move move = getColorsMove(board, turn);
        game = appendTurn(game, makeTurn(board, move));
        applyMove(board, move);
        turn = colorInv(turn);
    }while(lastTurn(game)->state != S_MATE);

    return 0;
}
