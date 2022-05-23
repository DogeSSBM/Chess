#ifndef EVALSTATE_H
#define EVALSTATE_H

Color stateTypeColor(const GameStateType type)
{
    return type & 1 ? C_BLACK : C_WHITE;
}

GameStateType colorToMate(const Color color)
{
    if(color == C_WHITE)
        return G_MATE_W;
    if(color == C_BLACK)
        return G_MATE_B;
    fwprintf(stderr, L"Error: cannot put color %ls in mate\n", ColorStr[color]);
    exit(EXIT_FAILURE);
    return G_NEUTRAL_W;
}

GameStateType colorToCheck(const Color color)
{
    if(color == C_WHITE)
        return G_CHECK_W;
    if(color == C_BLACK)
        return G_CHECK_B;
    fwprintf(stderr, L"Error: cannot put color %ls in check\n", ColorStr[color]);
    exit(EXIT_FAILURE);
    return G_NEUTRAL_W;
}

GameStateType colorToNeutral(const Color color)
{
    if(color == C_WHITE)
        return G_NEUTRAL_W;
    if(color == C_BLACK)
        return G_NEUTRAL_B;
    fwprintf(stderr, L"Error: no neutral state for color %ls\n", ColorStr[color]);
    exit(EXIT_FAILURE);
    return G_NEUTRAL_W;
}

bool isEndGame(const GameStateType type)
{
    return type > G_CHECK_B;
}

bool isCheck(const GameStateType type)
{
    return type == G_CHECK_W || type == G_CHECK_B;
}

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

bool movesCanEscapeCheck(GameState state, const Vec src)
{
    Valid moves;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec dst = {.x = x, .y = y};
            if(!getValidAt(moves, dst, true))
                continue;
            Turn turn = {.src.pos = src, .dst.pos = dst};
            GameState applied = state;
            boardCpy(applied.board, state.board);
            applyTurn(&applied, &turn);
            if(!inCheck(applied.board, state.playerTurn))
                return true;
        }
    }
    return false;
}

bool inCheckMate(GameState state)
{
    if(!inCheck(state.board, state.playerTurn))
        return false;

    Vec king;
    Vec rooks[2];
    if(state.playerTurn == C_BLACK){
        king.x=4;
        king.y=0;
        rooks[0].x=0;
        rooks[0].y=0;
        rooks[1].x=7;
        rooks[1].y=0;
    }else if(state.playerTurn == C_WHITE){
        king.x=4;
        king.y=7;
        rooks[0].x=0;
        rooks[0].y=7;
        rooks[1].x=7;
        rooks[1].y=7;
    }else{
        fwprintf(stderr, L"Error: player %ls cannot be in mate\n", ColorStr[state.playerTurn]);
        exit(EXIT_FAILURE);
    }

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec src = {.x = x, .y = y};
            const Piece srcPiece = boardAt(state.board, src);
            const Color srcColor = pieceColor(srcPiece);
            if(srcColor == C_NONE)
                continue;

            Valid moves;
            resetValid(moves);
            validMovesStateless(state.board, moves, src);
            if(movesCanEscapeCheck(state, src))
                return false;
        }
    }

    if(!getValidAt(state.moved, king, true) && !getValidAt(state.moved, rooks[0], true)){
        return true;
    }
    if(!getValidAt(state.moved, king, true) && !getValidAt(state.moved, rooks[1], true)){
        return true;
    }
    return true;
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

GameStateType evalGameState(GameState state)
{
    if(inCheck(state.board, state.playerTurn)){
        if(inCheckMate(state))
            return colorToMate(state.playerTurn);

        return colorToCheck(state.playerTurn);
    }
    AllValid all;
    const uint total = validAllMoves(state, all);
    if(total == 0)
        return G_DRAW;
    return colorToNeutral(state.playerTurn);
}

Turn* lastTurn(Turn *turns)
{
    if(!turns)
        return NULL;
    while(turns->next)
        turns = turns->next;
    return turns;
}

void printTurns(Turn *turns)
{
    uint i = 0;
    while(turns){
        wprintf(
            L"turn %u: %lc%ls -> %lc%ls\n",
            i,
            pwc[turns->src.piece], vecStrify(turns->src.pos),
            pwc[turns->dst.piece], vecStrify(turns->dst.pos)
        );
        i++;
        turns = turns->next;
    }
}

#endif /* end of include guard: EVALSTATE_H */
