#ifndef GAME_H
#define GAME_H

bool areCastilable(const wc p1, const wc p2)
{
    return  (p1 != p2) &&
            (pieceColor(p1) == pieceColor(p2)) &&
            (p1 == L'♜' || p1 == L'♖' || p2 == L'♜' || p2 == L'♖') &&
            (p1 == L'♚' || p1 == L'♔' || p2 == L'♚' || p2 == L'♔');
}

bool isValidMove(Board board, const Color current, const Move move)
{
    if(
        !moveInBounds(move) ||
        pieceColor(getAt(board, move.selected)) != current ||
        eqPairu(move.selected, move.targeted)
    )
        return false;

    Moves moves = {0};
    findValidMoves(board, moves, move.selected);
    return getMoveAt(moves, move.targeted) != M_INVALID;
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

bool confirmMove(Board board, const Color current, const Move move)
{
    clearTerm();
    printTurnLabel(current);
    printBoardMove(board, move);
    printConfirmPrompt();
    return getConfirm();
}

Move getColorsMove(Board board, const Color current)
{
    Moves moves = {0};
    Move move = {.type = M_INVALID};
    do{
        clearTerm();
        printTurnLabel(current);
        if(move.type == M_HALF){
            printBoardS(board, move.selected);
            printTargetPrompt();
        }else{
            move.type = M_INVALID;
            printBoard(board);
            printMovePrompt();
        }
        move = tryReadMove(move);
        if(move.type != M_INVALID && pieceColor(getAt(board, move.selected)) != current)
            move.type = M_INVALID;
        printMove(move);
    }while(
        !isValidMove(board, current, move) ||
        !confirmMove(board, current, move)
    );
    return move;
}

bool applyMove(Board board, const Move move)
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
