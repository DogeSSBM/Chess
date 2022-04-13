#ifndef GAME_H
#define GAME_H

bool targetInBounds(const Pairu target)
{
    return target.x <= 8 && target.y <= 8;
}

wc pieceAt(wc board[8][8], const Pairu target)
{
    if(!targetInBounds(target)){
        fwprintf(stderr, L"Error: target (%u, %u) out of bounds\n", target.x, target.y);
        exit(EXIT_FAILURE);
    }
    return board[target.y][target.x];
}

Color pieceColor(const wc piece)
{
    if(piece == L'♜' || piece == L'♞' || piece == L'♝' || piece == L'♛' || piece == L'♚' || piece == L'♟')
        return C_WHITE;
    if(piece == L'♖' || piece == L'♘' || piece == L'♗' || piece == L'♕' || piece == L'♔' || piece == L'♙')
        return C_BLACK;
    if(piece != L' '){
        fwprintf(stderr, L"Error: \"%l\" is not a valid piece\n", piece);
        exit(EXIT_FAILURE);
    }
    return C_NONE;
}

Color nextTurnColor(const Color current)
{
    if(current == C_WHITE)
        return C_BLACK;
    else if(current == C_BLACK)
        return C_WHITE;
    else{
        fwprintf(stderr, L"Error: current turn color is not valid color\n(int)current == %i\n", (int)current);
        exit(EXIT_FAILURE);
    }
    return C_NONE;
}

bool isValidChars(char *buf)
{
    if(!buf)
        return false;
    return  (strnlen(buf, 6) == 5) &&
            (buf[4] == '\n') &&
            (buf[0] >= 'a' && buf[0] <= 'h') &&
            (buf[2] >= 'a' && buf[2] <= 'h') &&
            (buf[1] >= '0' && buf[1] <= '8') &&
            (buf[3] >= '0' && buf[3] <= '8');
}

Move charsToMove(char *buf)
{
    return (const Move){
        .selected.x = buf[0]-'a',
        .selected.y = '8'-buf[1],
        .targeted.x = buf[2]-'a',
        .targeted.y = '8'-buf[3]
    };
}

bool moveInBounds(const Move move)
{
    return targetInBounds(move.selected) && targetInBounds(move.targeted);
}

Move getMove(void)
{
    char buf[32] = {0};
    const uint len = strnlen(fgets(buf, 6, stdin), 6);
    if(len == 5 && isValidChars(buf))
        return charsToMove(buf);
    if(len == 6 && buf[5] != '\n')
        while((fgetc(stdin))!='\n');
    return (const Move){.selected.x = 9, .selected.y = 9, .targeted.x = 9, .targeted.y= 9};
}

bool isValidMove(wc board[8][8], const Color current, const Move move)
{
    if(!moveInBounds(move) || pieceColor(pieceAt(board, move.selected)) != current)
        return false;
    return true;
}

Move getColorsMove(wc board[8][8], const Color current)
{
    Move move;
    do{
        clearTerm();
        printTurn(board, current);
        move = getMove();
    }while(!isValidMove(board, current, move));
    return move;
}

#endif /* end of include guard: GAME_H */
