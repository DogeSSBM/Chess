#ifndef GAME_H
#define GAME_H

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

bool isValidMove(wc board[8][8], const Color current, const Move move)
{
    if(
        move.type != M_VALID ||
        !moveInBounds(move) ||
        eqPairu(move.selected, move.targeted)
    )
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
    Move move = {.type = M_INVALID};
    do{
        clearTerm();
        printTurnLabel(current);
        if(move.type == M_INVALID){
            printBoard(board);
            printMovePrompt();
        }
        else if(move.type == M_HALF){
            printBoardS(board, move.selected);
            printTargetPrompt();
        }
        move = tryReadMove(move);
    }while(!isValidMove(board, current, move) || !confirmMove(board, current, move));
    return move;
}

bool applyMove(wc board[8][8], const Move move)
{
    const wc srcPiece = getAt(board, move.selected);
    const wc dstPiece = setAt(board, move.targeted, srcPiece);
    // if(areCastilable(srcPiece, dstPiece))
    //     setAt(board, move.selected, dstPiece);
    // else
        setAt(board, move.selected, L' ');
    return true;
}

#endif /* end of include guard: GAME_H */
