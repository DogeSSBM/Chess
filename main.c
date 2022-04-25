#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Board board;
    resetBoard(board);

    Color curColor = C_WHITE;
    Turn *game = NULL;
    do{
        printGame(game);
        const Move move = getColorsMove(board, game, curColor);
        game = makeMove(board, game, move);
        curColor = colorInv(curColor);
    }while(lastTurn(game)->state != S_MATE);

    return 0;
}
