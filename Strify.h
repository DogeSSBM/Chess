#ifndef STRIFY_H
#define STRIFY_H

void boardStrifyNoLines(Board board, BoardStr str)
{
    memset(str, 0, BOARDSTRLEN);
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    wcscat(str, L"  ---------------------------------\n");
    for(int y = 0; y < 8; y++){
        swprintf(str+wcslen(str), BOARDSTRLEN, L"%lc |", btowc('8'-y));
        for(int x = 0; x < 8; x++){
            swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc %lc", pwc[board[y][x]], x == 7 ? L'|' : L' ');
        }
        swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc \n", btowc('8'-y));
        if(y < 7)
            wcscat(str, L"  |                               |\n");
    }
    wcscat(str, L"  ---------------------------------\n");
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    str[BOARDSTRLEN-1] = '\0';
}

void boardStrifyMids(Board board, BoardStr str)
{
    memset(str, 0, BOARDSTRLEN);
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    wcscat(str, L"  ---------------------------------\n");
    for(int y = 0; y < 8; y++){
        swprintf(str+wcslen(str), BOARDSTRLEN, L"%lc |", btowc('8'-y));
        for(int x = 0; x < 8; x++){
            swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc %lc", pwc[board[y][x]], x == 7 ? L'|' : L' ');
        }
        swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc \n", btowc('8'-y));
        if(y < 7)
            wcscat(str, L"  |   +   +   +   +   +   +   +   |\n");
    }
    wcscat(str, L"  ---------------------------------\n");
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    str[BOARDSTRLEN-1] = '\0';
}

void boardStrify(Board board, BoardStr str)
{
    memset(str, 0, BOARDSTRLEN);
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    wcscat(str, L"  +---+---+---+---+---+---+---+---+\n");
    for(int y = 0; y < 8; y++){
        swprintf(str+wcslen(str), BOARDSTRLEN, L"%lc |", btowc('8'-y));
        for(int x = 0; x < 8; x++){
            swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc |", pwc[board[y][x]]);
        }
        swprintf(str+wcslen(str), BOARDSTRLEN, L" %lc \n", btowc('8'-y));
        wcscat(str, L"  +---+---+---+---+---+---+---+---+\n");
    }
    wcscat(str, L"    a   b   c   d   e   f   g   h  \n");
    str[BOARDSTRLEN-1] = '\0';
}

int wcToint(const wc c)
{
    if(wcValidNum(c))
        return L'8' - c;
    if(wcValidAlpha(c))
        return c - L'a';
    return -1;
}

int charToint(const char c)
{
    if(charValidNum(c))
        return L'8' - c;
    if(charValidAlpha(c))
        return c - L'a';
    return -1;
}

wc intTox(const int n)
{
    if(intValid(n))
        return L'a'+n;
    return L' ';
}

wc intToy(const int n)
{
    if(intValid(n))
        return L'8'-n;
    return L' ';
}

wc* vecStrify(const Vec pos)
{
    static wc buf[4][64];
    static uint i = 0;
    wc* cur = buf[i];
    swprintf(cur, 64, L"[%u]:{%i, %i}", i, pos.x, pos.y);
    i = (i+1)%4;
    return cur;
}

wc* vecStrifyIo(const Vec pos)
{
    static wc buf[4][64];
    static uint i = 0;
    wc* cur = buf[i];
    swprintf(cur, 64, L"[%u]:\"%lc%lc\"", i, intTox(pos.x), intToy(pos.y));
    i = (i+1)%4;
    return cur;
}

#endif /* end of include guard: STRIFY_H */
