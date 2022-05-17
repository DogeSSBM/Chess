#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Turn *turns = NULL;
    while(1){
        // cons game state
        GameState state = consGameState(turns);
        // eval board state
        // GameStateType stateType = evalGameState(state);
        BoardStr str;
        boardStrify(state.board, str);
        wprintf(L"%ls\n\n", str);
        boardStrifyMids(state.board, str);
        wprintf(L"%ls\n\n", str);
        boardStrifyNoLines(state.board, str);
        wprintf(L"%ls\n\n", str);
        return 0;
        // get player move
        // append move
    }

    return 0;
}
