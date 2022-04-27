#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Board board;
    resetBoard(board);

    Color curColor = C_WHITE;
    Turn *game = NULL;
    do{
        // const Move move = getColorsMove(board, game, curColor);
        Move move = {0};
        while(move->type < M_VALID){
            clearTerm();
            printTurnLabel(curColor);
            if(move.type == M_HALF){
                printBoardS(board, move.src);
                printTargetPrompt();
            }else{
                move.type = M_INVALID;
                printBoard(board);
                printMovePrompt();
            }
        }
        game = makeMove(board, game, move);
        curColor = colorInv(curColor);
    }while(lastTurn(game)->state != S_MATE);
    printGame(game);
    freeGame(game);
    return 0;
}
