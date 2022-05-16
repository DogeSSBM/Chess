#include "Includes.h"



int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    wprintf(L"wchars to ints -\n");
    for(uint i = 0; i < 8; i++)
        wprintf(L"wcToint('%lc') -> %u\n", L'a'+i, wcToint(L'a'+i));

    wprintf(L"\nint to wchars -\n");
    for(uint i = 0; i < 8; i++)
        wprintf(L"intTox(%u) -> '%lc' uintToy(%u) -> '%lc'\n", i, intTox(i), i, intToy(i));

    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Vec pos = {.x = x, .y = y};
            wprintf(L"%ls -> %ls\n", vecStrify(pos), vecStrifyIo(pos));
        }
    }


    // Turn *game = NULL;
    //
    // while(1){
    //     GameState state = consGameState(game);
    //     BoardStr str;
    //     boardStrify(state.board, str);
    //     selectValid(str, (const Vec2){.x=0,.y=0}, state.moved, L"  ", L"!!");
    //     wprintf(L"Moved -\n%ls\n", str);
    //     game = nextTurn(game, state);
    //     state.type = evalState(game);
    // }

    return 0;
}
