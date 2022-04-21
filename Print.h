#ifndef PRINT_H
#define PRINT_H

Color pieceColor(const wc);

void showMoves(Board board, const Pairu select)
{
    if(!pairuInBounds(select))
        return;
    Moves moves = {0};
    findValidMoves(board, moves, select);
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Pairu dst = {.x = x, .y = y};
            MoveType mt = getMoveAt(moves, dst);
            if(select.x == x && select.y == y)
                fputwc(getAt(board, select), stdout);
            else if(mt == M_INVALID)
                fputwc(L'#' ,stdout);
            else if(mt == M_VALID)
                fputwc(L' ' ,stdout);
            else if(mt == M_CAPTURE)
                fputwc(L'!' ,stdout);
        }
        fputwc(L'\n',stdout);
    }
    fputwc(L'\n',stdout);
}

void printBoardHelper(Board board, const bool colors, const Pairu select, const Pairu target)
{
    // showMoves(Board board, select);
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
    fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    for(int y = 0; y < 8; y++){
        fputwc(btowc('8'-y),stdout);
        fputws(L" |",stdout);
        for(int x = 0; x < 8; x++){
            const Pairu dst = {.x = x, .y = y};
            const bool selected = x == select.x && y == select.y;
            const bool targeted = x == target.x && y == target.y;

            wc l = L' ';
            wc r = L' ';
            if(selected){
                l = targeted?L'}':L'[';
                r = targeted?L'{':L']';
            }else if(targeted){
                l = L'>';
                r = L'<';
            }

            wc p = getAt(board, dst);
            fputwc(l,stdout);
            fputwc(p,stdout);
            fputwc(r,stdout);
            fputwc(L'|',stdout);
        }
        fputwc(L' ',stdout);
        fputwc(btowc('8'-y),stdout);
        fputwc(L'\n',stdout);
        fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    }
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
}

void printBoard(Board board)
{
    printBoardHelper(board, false, (const Pairu){.x=9,.y=9}, (const Pairu){.x=9,.y=9});
}

void printBoardS(Board board, const Pairu selectedPiece)
{
    printBoardHelper(board, false, selectedPiece, (const Pairu){.x=9,.y=9});
}

void printBoardMove(Board board, const Move move)
{
    printBoardHelper(board, false, move.selected, move.targeted);
}

void printBoardColors(Board board)
{
    printBoardHelper(board, true, (const Pairu){.x=9,.y=9}, (const Pairu){.x=9,.y=9});
}

void printTurnLabel(const Color turn)
{
    if(turn == C_WHITE)
        wprintf(L"White's turn -\n");
    else if(turn == C_BLACK)
        wprintf(L"Black's turn -\n");
    else{
        fwprintf(stderr, L"Error: turn is not valid color\n(int)turn == %i\n", (int)turn);
        exit(EXIT_FAILURE);
    }
}

void printMovePrompt(void)
{
    wprintf(L"Enter turn...\n");
}

void printTargetPrompt(void)
{
    wprintf(L"Enter target or nothing to clear...\n");
}

void printConfirmPrompt(void)
{
    wprintf(L"Confirm move (Y/n)...\n");
}

#endif /* end of include guard: PRINT_H */
