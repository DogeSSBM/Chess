#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");
    Turn *game = NULL;

    GameState state = G_NEUTRAL;
    while(state < G_DRAW){
        game = nextTurn(game, state);
        state = evalState(game);
    }

    return 0;
}
