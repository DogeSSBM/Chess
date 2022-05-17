#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Turn *turns = NULL;
    while(1){
        // cons game state
        GameState state = consGameState(turns);
        // eval board state
        GameStateType stateType = evalGameState(state);
        // get player move
        // append move
    }

    return 0;
}
