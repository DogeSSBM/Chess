#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Turn *turns = NULL;
    Color nextTurn = C_WHITE;
    while(1){
        // cons game state
        GameState state = consGameState(turns, nextTurn);
        // display board state
        BoardStr str;
        boardStrify(state.board, str);

        // eval board state
        // if(isEndGame(evalGameState(state)))
            // return 0;
        // get all possible moves
        AllValid all;
        validAllMoves(state, all);

        // get player move
        // append move
        Turn turn = turnRead(state.board, all, state.playerTurn);
        wprintf(
            L"%lc%ls -> %lc%ls\n",
            pwc[turn.src.piece], vecStrify(turn.src.pos),
            pwc[turn.dst.piece], vecStrify(turn.dst.pos)
        );
        turns = appendTurn(turns, turn);
        // change playerTurn color
        nextTurn = colorInv(nextTurn);
    }

    return 0;
}
