#ifndef IO_H
#define IO_H

size_t strnlen(const char* str, const size_t max)
{
  const char* end = memchr(str, '\0', max);
  return end ? (size_t)(end-str) : max;
}

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

void boardStrSelect(BoardStr str, const Vec pos, wc braces[2])
{
    const uint index = 37 + pos.y * 36 + (pos.y + 1) * 39 + 4 * pos.x;
    str[index-1] = braces[0];
    str[index+1] = braces[1];
}

void boardStrSelectValid(BoardStr str, Valid moves, wc braces[2])
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec pos = {.x = x, .y = y};
            if(getValidAt(moves, pos, true))
                boardStrSelect(str, pos, braces);
        }
    }
}

Color gameStateColor(const GameStateType type)
{
    if(type == G_NEUTRAL_W || type == G_CHECK_W || type == G_MATE_W)
        return C_WHITE;
    if(type == G_NEUTRAL_B || type == G_CHECK_B || type == G_MATE_B)
        return C_BLACK;
    return C_NONE;
}

void gameStatePrintPrompt(const GameStateType type)
{
    const Color color = gameStateColor(type);
    if(color == C_WHITE)
        wprintf(L"White's turn -\n");
    else if(color == C_BLACK)
        wprintf(L"Black's turn -\n");
    else{
        fwprintf(stderr, L"Error: state invalid\n");
        exit(EXIT_FAILURE);
    }
}

Vec vecParse(const Input in)
{
    Vec ret = {.x = -1, .y = -1};
    if(!in.str || strnlen(in.str, 2) != 2)
        return ret;
    if((ret.x = charToint(in.str[0])) == -1)
        return ret;
    ret.y = charToint(in.str[1]);
    return ret;
}

Input readInput(void)
{
    Input in = {0};
    if(!fgets(in.str, 512, stdin)){
        fwprintf(stderr, L"Error: fgets returned NULL\n");
        exit(EXIT_FAILURE);
    }
    uint buflen = strnlen(in.str, 512);
    if(buflen == 512){
        while(in.str[511] != '\n')
            in.str[511] = fgetc(stdin);
        in.str[0] = L'\0';
        return in;
    }
    return in;
}

#endif /* end of include guard: IO_H */
