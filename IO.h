#ifndef IO_H
#define IO_H
wc buf[64];

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

void boardStrify(Board board, BoardStr str)
{
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    wcscat(str, L"  +---+---+---+---+---+---+---+---+\n");
    for(int y = 0; y < 8; y++){
        swprintf(str+wcslen(str), 708, L"%lc |", btowc('8'-y));
        for(int x = 0; x < 8; x++){
            swprintf(str+wcslen(str), 708, L" %lc |", pwc[board[y][x]]);
        }
        swprintf(str+wcslen(str), 708, L" %lc \n", btowc('8'-y));
        wcscat(str, L"  +---+---+---+---+---+---+---+---+\n");
    }
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
}

wc* Vec2Strify(const Vec2 pos)
{
    swprintf(buf, 64, L"(Vec2){%i, %i}", pos.x, pos.y);
    return buf;
}

#endif /* end of include guard: IO_H */
