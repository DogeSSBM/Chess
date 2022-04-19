#ifndef BOARD_H
#define BOARD_H

bool pairuInBounds(const Pairu target)
{
    return target.x < 8 && target.y < 8;
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

wc getAt(wc board[8][8], const Pairu target)
{
    if(!pairuInBounds(target)){
        fwprintf(stderr, L"Error: target (%u, %u) out of bounds\n", target.x, target.y);
        exit(EXIT_FAILURE);
    }
    return board[target.y][target.x];
}

wc setAt(wc board[8][8], const Pairu target, const wc piece)
{
    if(!pairuInBounds(target)){
        fwprintf(stderr, L"Error: target (%u, %u) out of bounds\n", target.x, target.y);
        exit(EXIT_FAILURE);
    }
    const wc prev = getAt(board, target);
    board[target.y][target.x] = piece;
    return prev;
}

Color pieceColor(const wc piece)
{
    if(
        piece == L'♜' || piece == L'♞' || piece == L'♝' ||
        piece == L'♛' || piece == L'♚' || piece == L'♟'
    )
        return C_WHITE;

    if(
        piece == L'♖' || piece == L'♘' || piece == L'♗' ||
        piece == L'♕' || piece == L'♔' || piece == L'♙'
    )
        return C_BLACK;

    if(piece != L' '){
        fwprintf(stderr, L"Error: \'%lc\' is not a valid piece\n", piece);
        exit(EXIT_FAILURE);
    }

    return C_NONE;
}

bool moveInBounds(const Move move)
{
    return pairuInBounds(move.selected) && pairuInBounds(move.targeted);
}

Move readTarget(Move move)
{
    move.targeted.x = 9;
    move.targeted.y = 9;
    int buf[3] = {'\0'};
    uint i = 0;
    while(i < 3){
        buf[i] = fgetc(stdin);
        if(buf[i] == '\n')
            break;
        i++;
    }
    if(i == 0){
        move.selected.x = 9;
        move.selected.y = 9;
        move.targeted.x = 9;
        move.targeted.y = 9;
    }else if(i == 2){
        move.targeted.x = coordToUint(buf[0]);
        move.targeted.y = coordToUint(buf[1]);
    }else if(i >= 3 && buf[2] != '\n'){
        while(fgetc(stdin) != '\n');
    }

    return move;
}

Move readMove(void)
{
    Move ret = {
        .selected.x = 9, .selected.y = 9,
        .targeted.x = 9, .targeted.y = 9
    };
    int buf[5] = {'\0'};
    uint i = 0;
    while(i < 5){
        buf[i] = fgetc(stdin);
        if(buf[i] == '\n')
            break;
        i++;
    }
    if(i == 4){
        ret.targeted.x = coordToUint(buf[2]);
        ret.targeted.y = coordToUint(buf[3]);
    }
    if(i == 2 || i == 4){
        ret.selected.x = coordToUint(buf[0]);
        ret.selected.y = coordToUint(buf[1]);
    }else if(i >= 5 && buf[4] != '\n')
        while(fgetc(stdin) != '\n');

    return ret;
}

#endif /* end of include guard: BOARD_H */
