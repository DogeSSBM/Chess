#ifndef GAME_H
#define GAME_H

bool isValidMove(Board board, Turn *game, const Color current, Move *move)
{
    if(
        !moveInBounds(*move) ||
        pieceColor(getAt(board, move->src)) != current ||
        eqPairu(move->src, move->dst)
    )
        return false;

    Moves moves = {0};
    findValidMoves(board, game, moves, move->src);
    if(getMoveAt(moves, move->dst) == M_PASSANT)
        move->type = M_PASSANT;
    return getMoveAt(moves, move->dst) != M_INVALID;
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

// Turn* lastTurn(Turn *); // i know... this is hax
Move getColorsMove(Board board, Turn *game, const Color current)
{
    Move move = {.type = M_INVALID};
    do{
        if(move.type == M_VALID)
            return move;

        clearTerm();
        // printMove(move);
        printTurnLabel(current);

        if(move.type == M_HALF){
            printBoardS(board, move.src);
            printTargetPrompt();
        }else{
            move.type = M_INVALID;
            printBoard(board);
            printMovePrompt();
        }

        move = tryReadMove(move);
        if(
            move.type != M_INVALID &&
            pieceColor(getAt(board, move.src)) != current
        )
            move.type = M_INVALID;

    }while(
        !isValidMove(board, game, current, &move) ||
        !confirmMove(board, current, move)
    );

    return move;
}

#endif /* end of include guard: GAME_H */
