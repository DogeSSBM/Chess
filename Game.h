#ifndef GAME_H
#define GAME_H

bool pairuInBounds(const Pairu target)
{
    return target.x <= 8 && target.y <= 8;
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

bool areCastilable(const wc p1, const wc p2)
{
    return  (p1 != p2) &&
            (pieceColor(p1) == pieceColor(p2)) &&
            (p1 == L'♜' || p1 == L'♖' || p2 == L'♜' || p2 == L'♖') &&
            (p1 == L'♚' || p1 == L'♔' || p2 == L'♚' || p2 == L'♔');
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

bool isValidAlpha(const char c)
{
    return c >= 'a' && c <= 'h';
}

bool isValidNum(const char c)
{
    return c >= '0' && c <= '8';
}

uint coordToUint(const char c)
{
    if(isValidNum(c))
        return '8' - c;
    if(isValidAlpha(c))
        return c - 'a';
    return 9;
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
    return pairuInBounds(move.selected) && pairuInBounds(move.targeted);
}

Pairu readCoord(const bool final)
{
    Pairu ret = {.x = 9, .y = 9};
    int c;
    if((c = fgetc(stdin)) == '\n')

        return ret;
    ret.x = coordToUint(c);
    if((c = fgetc(stdin)) == '\n')
        return ret;
    if(final && fgetc(stdin) != '\n')
        return ret;
    ret.y = coordToUint(c);
    return ret;
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
    //
    // char buf[32] = {0};
    // const uint len = strnlen(fgets(buf, 6, stdin), 6);
    // if(len == 5 && isValidChars(buf))
    //     return charsToMove(buf);
    // if(len == 6 && buf[5] != '\n')
    //     while((fgetc(stdin))!='\n');
    // return (const Move){
    //     .selected.x = 9, .selected.y = 9,
    //     .targeted.x = 9, .targeted.y = 9
    // };
}

bool isValidMove(wc board[8][8], const Color current, const Move move)
{
    if(!moveInBounds(move) || eqPairu(move.selected, move.targeted))
        return false;
    const wc srcPiece = getAt(board, move.selected);
    const wc dstPiece = getAt(board, move.targeted);
    if(srcPiece == L' ' || pieceColor(srcPiece) != current)
        return false;
    if(pieceColor(dstPiece) == current)
        return areCastilable(srcPiece, dstPiece);
    return true;
}

bool getConfirm(void)
{
    int ans = fgetc(stdin);
    if(ans == '\n')
        return true;
    while((fgetc(stdin))!='\n')
            ans = 'n';
    return ans == 'y' || ans == 'Y';
}

bool confirmMove(wc board[8][8], const Color current, const Move move)
{
    clearTerm();
    printTurnLabel(current);
    printBoardMove(board, move);
    printConfirmPrompt();
    return getConfirm();
}

Move getColorsMove(wc board[8][8], const Color current)
{
    Move move = {
        .selected.x = 9, .selected.y = 9,
        .targeted.x = 9, .targeted.y = 9
    };

    do{
        clearTerm();
        printTurnLabel(current);
        printBoard(board);
        printMovePrompt();
        move = readMove();

        if(
            isValidMove(board, current, move) &&
            confirmMove(board, current, move)
        )
            return move;

        while(
            pairuInBounds(move.selected) &&
            pieceColor(getAt(board, move.selected)) == current &&
            !pairuInBounds(move.targeted)
        ){
            clearTerm();
            printTurnLabel(current);
            printBoardS(board, move.selected);
            printTargetPrompt();
            move = readTarget(move);

            if(
                isValidMove(board, current, move) &&
                confirmMove(board, current, move)
            )
                return move;
        }
    }while(!isValidMove(board, current, move) || !confirmMove(board, current, move));
    return move;
}

bool applyMove(wc board[8][8], const Move move)
{
    const wc srcPiece = getAt(board, move.selected);
    const wc dstPiece = setAt(board, move.targeted, srcPiece);
    if(areCastilable(srcPiece, dstPiece))
        setAt(board, move.selected, dstPiece);
    else
        setAt(board, move.selected, L' ');
    return true;
}

#endif /* end of include guard: GAME_H */
