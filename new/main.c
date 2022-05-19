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
        wprintf(L"%ls\n\n", str);
        // eval board state
        const GameStateType type = evalGameState(state);

        AllValid all = {0};
        validAllMoves(state, all, state.playerTurn);
        return 0;
        // get player move
        // append move
        // change playerTurn color
        nextTurn = colorInv(nextTurn);
    }

    return 0;
}
