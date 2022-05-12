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

void selectValid(BoardStr str, const Vec2 src, Valid moves, wc srcBraces[2], wc validBraces[2])
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec2 pos = {.x = x, .y = y};
            if(eqVec2(pos, src))
                boardStrSelect(str, pos, srcBraces);
            if(getValidAt(moves, pos, true))
                boardStrSelect(str, pos, validBraces);
        }
    }
}

bool isValidAlpha(const wc c)
{
    return c >= L'a' && c <= L'h';
}

bool isValidNum(const wc c)
{
    return c >= L'0' && c <= L'8';
}

bool isValidUint(const uint n)
{
    return n <= 8;
}

uint coordToUint(const wc c)
{
    if(isValidNum(c))
        return L'8' - c;
    if(isValidAlpha(c))
        return c - L'a';
    return 9;
}

wc uintTox(const uint n)
{
    if(isValidUint(n))
        return L'a'+n;
    return L' ';
}

wc uintToy(const uint n)
{
    if(isValidUint(n))
        return L'8'-n;
    return L' ';
}

void printInputPrompt(const Input in)
{
    switch(in.type){
        case I_INVALID:
            wprintf(L"Enter turn...\n");
            break;
        case I_HALF:
            wprintf(
                L"Enter dst -\nsrc %lc (%lc,%lc)...\n",
                pwc[in.turn.src.piece],
                uintTox(in.turn.src.pos.x),
                uintToy(in.turn.src.pos.y)
            );
            break;
        case I_FULL:
            wprintf(
                L"Confirm move -\nsrc: %lc (%lc,%lc) -> dst: %lc (%lc,%lc)\n(Y/n)...\n",
                in.turn.src.piece,
                uintTox(in.turn.src.pos.x),
                uintToy(in.turn.src.pos.y),
                in.turn.dst.piece,
                uintTox(in.turn.dst.pos.x),
                uintToy(in.turn.dst.pos.y)
            );
            break;
        default:
            fwprintf(
                stderr,
                L"Invalid input -\nsrc: %lc (%lc,%lc) -> dst: %lc (%lc,%lc)\nEnter turn...\n",
                in.turn.src.piece,
                uintTox(in.turn.src.pos.x),
                uintToy(in.turn.src.pos.y),
                in.turn.dst.piece,
                uintTox(in.turn.dst.pos.x),
                uintToy(in.turn.dst.pos.y)
            );
            break;
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

bool validHalf(const char *buf)
{
    if(strnlen(buf, 6) != 3)
        return false;
    for(uint i = 0; i < 2; i++){
        if(coordToUint(buf[i]) == 9)
            return false;
    }
    return buf[4] != '\n';
}

bool validFull(const char *buf)
{
    if(strnlen(buf, 6) != 5)
        return false;
    for(uint i = 0; i < 4; i++){
        if(coordToUint(buf[i]) == 9)
            return false;
    }
    return buf[4] != '\n';
}

Vec2 bufToVec2(const char *buf)
{
    return (const Vec2){
        .x = coordToUint(buf[0]),
        .y = coordToUint(buf[1])
    };
}

Input getInput(Input in, Board board, const GameStateType type)
{
    const Color color = gameStateColor(type);
    if(color == C_NONE){
        fwprintf(stderr, L"Error: cannot get input for color C_NONE\n");
        exit(EXIT_FAILURE);
    }
    printInputPrompt(in);
    char buf[6] = {'\0'};
    if(!fgets(buf, 6, stdin)){
        fwprintf(stderr, L"Error: fgets returned NULL\n");
        exit(EXIT_FAILURE);
    }

    uint buflen = strnlen(buf, 6);
    if(buflen == 6){
        while(buf[5] != '\n')
            buf[5] = fgetc(stdin);
        in.type = I_INVALID;
        return in;
    }

    fputs("Henlo!?!?\n", stdout);
    printf("buf: \"%6s\"\n", buf);
    fflush(stdout);

    switch(in.type){
        case I_VALID:
            return in;
            break;
        case I_INVALID:
            if(
                buflen == 3 &&
                validHalf(buf)
            ){
                in.type = I_HALF;
                in.turn.src.pos  = bufToVec2(buf);
                in.turn.src.piece = pieceAt(board, in.turn.src.pos);
                return in;
            }
            if(
                buflen == 5 &&
                validFull(buf)
            ){
                in.type = I_FULL;
                in.turn.src.pos  = bufToVec2(buf);
                in.turn.dst.pos  = bufToVec2(&buf[2]);
                in.turn.src.piece = pieceAt(board, in.turn.src.pos);
                in.turn.dst.piece = pieceAt(board, in.turn.dst.pos);
                return in;
            }
            break;
        case I_HALF:
            if(
                buflen == 3 &&
                validHalf(buf)
            ){
                in.type = I_FULL;
                in.turn.dst.pos  = bufToVec2(buf);
                in.turn.dst.piece = in.turn.src.piece;
                return in;
            }
            break;
        case I_FULL:
            if(
                ((buflen == 2 && (buf[0] == 'y' || buf[0] == 'Y') && buf[1] == '\n') ||
                (buflen == 1 && buf[0] == '\n'))&&
                pieceColor(in.turn.src.piece) == color
            ){
                in.type = I_VALID;
                return in;
            }
            break;
        default:
            fwprintf(stderr, L"Error: input default switch case\nsrc: %lc (%lc,%lc) -> dst: %lc (%lc,%lc)\nEnter turn...\n",
            in.turn.src.piece,
            uintTox(in.turn.src.pos.x),
            uintToy(in.turn.src.pos.y),
            in.turn.dst.piece,
            uintTox(in.turn.dst.pos.x),
            uintToy(in.turn.dst.pos.y)
        );
            exit(EXIT_FAILURE);
            break;
    }
    in.type = I_INVALID;
    return in;
}

Input getTurnInput(Board board, const GameStateType type)
{
    Input in = {0};
    while(
        (in = getInput(in, board, type)).type != I_VALID ||
        !validPos(in.turn.src.pos, false) ||
        !validPos(in.turn.dst.pos, false)
    );
    return in;
}

#endif /* end of include guard: IO_H */
