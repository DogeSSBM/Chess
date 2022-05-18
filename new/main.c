#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Turn *turns = NULL;
    while(1){
        // cons game state
        GameState state = consGameState(turns);
        // display board state
        BoardStr str;
        boardStrify(state.board, str);
        wprintf(L"%ls\n\n", str);
        // eval board state
        const GameStateType type = evalGameState(state);

        while()
        // get player move
        // append move
    }

    return 0;
}
