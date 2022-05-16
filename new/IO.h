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
                intTox(in.turn.src.pos.x),
                intToy(in.turn.src.pos.y)
            );
            break;
        case I_FULL:
            wprintf(
                L"Confirm move -\nsrc: %lc (%lc,%lc) -> dst: %lc (%lc,%lc)\n(Y/n)...\n",
                in.turn.src.piece,
                intTox(in.turn.src.pos.x),
                intToy(in.turn.src.pos.y),
                in.turn.dst.piece,
                intTox(in.turn.dst.pos.x),
                intToy(in.turn.dst.pos.y)
            );
            break;
        default:
            fwprintf(
                stderr,
                L"Error: invalid prompt\ntype: %ls\nsrc: %lc {%lc,%lc}\ndst: %lc {%lc,%lc}\n",
                InputTypeStr[in.type],
                in.turn.src.piece,
                intTox(in.turn.src.pos.x),
                intToy(in.turn.src.pos.y),
                in.turn.dst.piece,
                intTox(in.turn.dst.pos.x),
                intToy(in.turn.dst.pos.y)
            );
            exit(EXIT_FAILURE);
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
        if(wcToint(buf[i]) == 9)
            return false;
    }
    return buf[4] != '\n';
}

bool validFull(const char *buf)
{
    if(strnlen(buf, 6) != 5)
        return false;
    for(uint i = 0; i < 4; i++){
        if(wcToint(buf[i]) == 9)
            return false;
    }
    return buf[4] != '\n';
}

Vec bufToVec(const char *buf)
{
    return (const Vec){
        .x = wcToint(buf[0]),
        .y = wcToint(buf[1])
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


    switch(in.type){
        case I_INVALID:
            if(
                buflen == 3 &&
                validHalf(buf)
            ){
                in.type = I_HALF;
                in.turn.src.pos  = bufToVec(buf);
                in.turn.src.piece = boardAt(board, in.turn.src.pos);
                return in;
            }
            if(
                buflen == 5 &&
                validFull(buf)
            ){
                in.type = I_FULL;
                in.turn.src.pos  = bufToVec(buf);
                in.turn.dst.pos  = bufToVec(&buf[2]);
                in.turn.src.piece = boardAt(board, in.turn.src.pos);
                in.turn.dst.piece = boardAt(board, in.turn.dst.pos);
                return in;
            }
            break;
        case I_HALF:
            if(
                buflen == 3 &&
                validHalf(buf)
            ){
                in.type = I_FULL;
                in.turn.dst.pos  = bufToVec(buf);
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
            fwprintf(stderr, L"Error: input default switch case\n");
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
