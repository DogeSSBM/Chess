#ifndef PRINT_H
#define PRINT_H

void printBoard(wc board[8][8])
{
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
    fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    for(int y = 0; y < 8; y++){
        fputwc(btowc('8'-y),stdout);
        fputws(L" |",stdout);
        for(int x = 0; x < 8; x++){
            fputwc(L' ',stdout);
            fputwc(board[y][x],stdout);
            fputwc(L' ',stdout);
            fputwc(L'|',stdout);
        }
        fputwc(L' ',stdout);
        fputwc(btowc('8'-y),stdout);
        fputwc(L'\n',stdout);
        fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    }
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
}

void printBoardColors(wc board[8][8])
{
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
    fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    for(int y = 0; y < 8; y++){
        fputwc(btowc('8'-y),stdout);
        fputws(L" |",stdout);
        for(int x = 0; x < 8; x++){
            fputwc(L' ',stdout);
            switch(pieceColor(board[y][x])){
                case C_WHITE:
                    fputwc(L'W',stdout);
                    break;
                case C_BLACK:
                    fputwc(L'B',stdout);
                    break;
                case C_NONE:
                default:
                    fputwc(L' ',stdout);
                    break;
            }
            fputwc(L' ',stdout);
            fputwc(L'|',stdout);
        }
        fputwc(L' ',stdout);
        fputwc(btowc('8'-y),stdout);
        fputwc(L'\n',stdout);
        fputws(L"  +---+---+---+---+---+---+---+---+\n",stdout);
    }
    fputws(L"    a   b   c   d   e   f   g   h  \n",stdout);
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
