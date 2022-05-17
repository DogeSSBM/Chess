#ifndef EVALSTATE_H
#define EVALSTATE_H

Vec getKing(Board board, const Color srcColor)
{
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: cannot get C_BLACK king\n");
        exit(EXIT_FAILURE);
    }
    const Piece srcPiece = srcColor == C_WHITE ? P_KING_W : P_KING_B;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec pos = {.x = x, .y = y};
            if(srcPiece == boardAt(board, pos))
                return pos;
        }
    }
    fwprintf(stderr, L"Error: could not find player %ls's king\n", ColorStr[srcColor]);
    exit(EXIT_FAILURE);
    return (const Vec){.x=-1, .y=-1};
}

bool inCheck(Board board, const Color srcColor)
{
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: player %ls cannot be in check\n", ColorStr[srcColor]);
        exit(EXIT_FAILURE);
    }
    const Vec king = getKing(board, srcColor);

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec src = {.x = x, .y = y};
            const Piece srcPiece = boardAt(board, src);
            const Color srcColor = pieceColor(srcPiece);
            if(srcColor == C_NONE)
                continue;

            Valid moves;
            resetValid(moves);
            validMovesStateless(board, moves, src);
            if(getValidAt(moves, king, true))
                return true;
        }
    }
    return false;
}

GameStateType evalGameState( GameState state)
{
    if(inCheck(state.board, state.playerTurn)){

    }
}

Turn* lastTurn(Turn *game)
{
    if(!game)
        return NULL;
    while(game->next)
        game = game->next;
    return game;
}

#endif /* end of include guard: EVALSTATE_H */
