#include "Includes.h"

int main(void)
{
    setlocale(LC_ALL, "en_US.utf8");

    Vec2 pos = {.x=0, .y=0};
    for(Dir d = D_U; d < D_N; d++){
        const Vec2 tmp = shift(pos, d, 1);
        wprintf(L"shift(%ls, %ls, 1) -> ", Vec2Strify(pos), DirStr[d]);
        wprintf(L"%ls\n",Vec2Strify(tmp));
    }

    return 0;
}
