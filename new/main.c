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
        // const GameStateType type = evalGameState(state);

        for(uint i = 0; i < G_N; i++){
            const GameStateType type = i;
            if(isEndGame(type))
                wprintf(L"%ls... \n%ls is the winner!\n", GameStateTypeStr[type], ColorStr[stateTypeColor(type)]);
        }

        return 0;



        // get player move
        // append move
    }

    return 0;
}
