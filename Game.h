#ifndef GAME_H
#define GAME_H

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

#endif /* end of include guard: GAME_H */
