#ifndef PRINT_H
#define PRINT_H

void printBoardHelper(wc board[8][8], const bool colorsOnly, const Pairu select, const Pairu target)
{
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
    fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    for(int y = 0; y < 8; y++){
        fputwc(btowc('8'-y),stdout);
        fputws(L" |",stdout);
        for(int x = 0; x < 8; x++){
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
            wc p = board[y][x];
            if(colorsOnly)
                p = pieceColor(board[y][x]) == C_WHITE?L'W':L'B';
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
void printBoard(wc board[8][8])
{
    printBoardHelper(board, false, (const Pairu){.x=9,.y=9}, (const Pairu){.x=9,.y=9});
}

void printBoardS(wc board[8][8], const Pairu selectedPiece)
{
    printBoardHelper(board, false, selectedPiece, (const Pairu){.x=9,.y=9});
}

void printBoardST(wc board[8][8], const Pairu selectedPiece, const Pairu targetedPiece)
{
    printBoardHelper(board, false, selectedPiece, targetedPiece);
}

void printBoardColors(wc board[8][8])
{
    printBoardHelper(board, true, (const Pairu){.x=9,.y=9}, (const Pairu){.x=9,.y=9});
}

void printTurn(const Color turn)
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

#endif /* end of include guard: PRINT_H */
