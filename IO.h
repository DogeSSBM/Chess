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

void playerTurnPrompt(const Color playerTurn)
{
    if(playerTurn == C_NONE){
        fwprintf(stderr, L"Error: color invalid\n");
        exit(EXIT_FAILURE);
    }
    wprintf(L"%ls's turn...\n", ColorStr[playerTurn]);
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

bool isValidTurn(AllValid all, Turn turn)
{
    if(!vecValid(turn.src.pos, false) || !vecValid(turn.dst.pos, false))
        return false;
    return getValidAt(all[turn.src.pos.x][turn.src.pos.y], turn.dst.pos, true);
}

Turn turnRead(Board board, AllValid all, const Color playerTurn)
{
    playerTurnPrompt(playerTurn);
    BoardStr str;
    Turn turn;
    do{
        boardStrify(board, str);
        do{
            wprintf(L"%ls\n\n", str);
            wprintf(L"Select a piece...\n");
            turn.src.pos = vecParse(readInput());
            turn.src.piece = boardAt(board, turn.src.pos);
        }while(pieceColor(turn.src.piece) != playerTurn);
        boardStrSelect(str, turn.src.pos, L"[]");
        boardStrSelectValid(str, all[turn.src.pos.x][turn.src.pos.y], L"><");
        wprintf(L"%ls\n\n", str);
        wprintf(L"Select destination...\n");
        turn.dst.pos = vecParse(readInput());
        turn.dst.piece = boardAt(board, turn.dst.pos);
    }while(!isValidTurn(all, turn));
    return turn;
}

void validAllPrint(Board board, AllValid all, const Color playerTurn)
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec src = {.x = x, .y = y};
            const Piece srcPiece = boardAt(board, src);
            if(srcPiece != P_EMPTY && pieceColor(srcPiece) == playerTurn){
                BoardStr str;
                boardStrify(board, str);
                boardStrSelect(str, src, L"[]");
                boardStrSelectValid(str, all[x][y], L"><");
                wprintf(
                    L"Piece %ls: %lc - %ls\n%ls\n",
                    PieceStr[srcPiece],
                    pwc[srcPiece],
                    vecStrify(src),
                    str
                );
            }
        }
    }
}

#endif /* end of include guard: IO_H */
