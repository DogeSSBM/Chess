#include "Includes.h"



int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");
    Turn *game = NULL;

    while(1){
        GameState state = consGameState(game);
        BoardStr str;
        boardStrify(state.board, str);
        selectValid(str, (const Vec2){.x=0,.y=0}, state.moved, L"  ", L"!!");
        wprintf(L"Moved -\n%ls\n", str);
        game = nextTurn(game, state);
        state.type = evalState(game);
    }

    return 0;
}
