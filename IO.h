#ifndef IO_H
#define IO_H
wc buf[64];

size_t strnlen(const char* str, const size_t max)
{
  const char* end = memchr(str, '\0', max);
  return end ? (size_t)(end-str) : max;
}

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

void boardStrify(Board board, BoardStr str)
{
    memset(str, 0, BOARDSTRLEN);
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

wc* vec2Strify(const Vec2 pos)
{
    swprintf(buf, 64, L"(Vec2){%i, %i}", pos.x, pos.y);
    return buf;
}

void boardStrSelect(BoardStr str, const Vec2 pos, wc braces[2])
{
    const uint index = 37 + pos.y * 36 + (pos.y + 1) * 39 + 4 * pos.x;
    str[index-1] = braces[0];
    str[index+1] = braces[1];
}

bool isValidAlpha(const wc c)
{
    return c >= L'a' && c <= L'h';
}

bool isValidNum(const wc c)
{
    return c >= L'0' && c <= L'8';
}

uint coordToUint(const wc c)
{
    if(isValidNum(c))
        return L'8' - c;
    if(isValidAlpha(c))
        return c - L'a';
    return 9;
}

void printMovePrompt(void)
{
    wprintf(L"Enter turn...\n");
}

void printConfirmPrompt(void)
{
    wprintf(L"Confirm move (Y/n)...\n");
}

Color gameStateColor(const GameState state)
{
    if(state == G_NEUTRAL_W || state == G_CHECK_W || state == G_MATE_W)
        return C_WHITE;
    if(state == G_NEUTRAL_B || state == G_CHECK_B || state == G_MATE_B)
        return C_BLACK;
    return C_NONE;
}

void gameStatePrintPrompt(const GameState state)
{
    const Color color = gameStateColor(state);
    if(color == C_WHITE)
        wprintf(L"White's turn -\n");
    else if(color == C_BLACK)
        wprintf(L"Black's turn -\n");
    else{
        fwprintf(stderr, L"Error: state invalid\n");
        exit(EXIT_FAILURE);
    }
}

bool getConfirm(void)
{
    printConfirmPrompt();

    int ans = fgetc(stdin);
    int nxt;
    if(ans == '\n')
        return true;
    if((ans == 'y' || ans == 'Y') && (nxt = fgetc(stdin)) == '\n')
        return true;
    while(nxt != '\n')
        nxt = fgetc(stdin);
    return false;
}

// a3b3n :5
bool getTurnInput(Turn *turn)
{
    printMovePrompt();
    char buf[6] = {'\0'};
    if(!fgets(buf, 6, stdin)){
        fwprintf(stderr, L"Error: input\n");
        exit(EXIT_FAILURE);
    }

    uint i = strnlen(buf, 6);
    if(i < 5)
        return false;
    if(i == 6){
        int c = buf[5];
        while(c != '\n')
            c = fgetc(stdin);
        return false;
    }
    if(isValidAlpha(buf[0]) && isValidNum(buf[1]) && isValidAlpha(buf[2]) && isValidNum(buf[3])){
        turn->src.pos.x = coordToUint(buf[0]);
        turn->src.pos.y = coordToUint(buf[1]);
        turn->dst.pos.x = coordToUint(buf[2]);
        turn->dst.pos.y = coordToUint(buf[3]);
        return validPos(turn->src.pos) && validPos(turn->dst.pos);
    }
    return false;
}

#endif /* end of include guard: IO_H */
