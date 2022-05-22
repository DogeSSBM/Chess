#ifndef CONSSTATE_H
#define CONSSTATE_H

bool isEnPassant(Board board, Turn turn)
{
    if(boardAt(board, turn.dst.pos) != P_EMPTY)
        return false;
    if(
        (turn.src.piece == P_PAWN_B && turn.dst.piece == P_PAWN_B) ||
        (turn.src.piece == P_PAWN_W && turn.dst.piece == P_PAWN_W)
    ){
        return turn.src.pos.x != turn.dst.pos.x;
    }
    return false;
}

void applyPassant(Board board, Turn turn)
{
    const Piece capPiece = turn.src.piece == P_PAWN_B ? P_PAWN_W : P_PAWN_B;
    const Dir bdir = turn.src.piece == P_PAWN_B ? D_U : D_D;
    if(boardSet(board, shift(turn.dst.pos, bdir, 1), P_EMPTY) != capPiece){
        fwprintf(stderr, L"Error: enpassant error");
        exit(EXIT_FAILURE);
    }
}

bool isCastle(Turn turn)
{
    return (
        turn.src.piece == P_KING_B && turn.dst.piece == P_ROOK_B
    ) || (
        turn.src.piece == P_KING_W && turn.dst.piece == P_ROOK_W
    );
}

/*
move king two squares toward rook.
move rook to square crossed by king.
*/
void applyCastle(Board board, Turn turn)
{
    const Dir rookDir = turn.dst.pos.x == 0 ? D_L : D_R;
    boardSet(board, shift(turn.src.pos, rookDir, 2), turn.src.piece);
    boardSet(board, shift(turn.src.pos, rookDir, 1), turn.dst.piece);
    boardSet(board, turn.dst.pos, P_EMPTY);
}

void applyMove(Board board, Turn turn)
{
    bool castle = false;
    if(isEnPassant(board, turn))
        applyPassant(board, turn);
    else if((castle = isCastle(turn)))
        applyCastle(board, turn);
    if(boardSet(board, turn.src.pos, P_EMPTY) != turn.src.piece){
        fwprintf(
            stderr,
            L"Error: board piece: '%lc' doesn't match move piece: '%lc' at: %ls\n",
            pwc[boardAt(board, turn.src.pos)],
            pwc[turn.src.piece],
            vecStrify(turn.src.pos)
        );
        exit(EXIT_FAILURE);
    }
    if(!castle)
        boardSet(board, turn.dst.pos, turn.dst.piece);
}

Turn *applyTurn(GameState state, Turn *turn)
{
    if(!turn)
        return NULL;
    setValidAt(state.moved, turn->src.pos, true, true);
    setValidAt(state.moved, turn->dst.pos, true, true);
    applyMove(state.board, *turn);

    return turn->next;
}

GameState consGameState(Turn *turns, const Color color)
{
    GameState ret = {
        .turns = turns,
        .playerTurn = color
    };
    resetBoard(ret.board);
    resetValid(ret.moved);
    while(turns){
        if(!turns->next)
            ret.last = turns;
        turns = applyTurn(ret, turns);
    }
    return ret;
}

Turn* appendTurn(Turn *turns, Turn turn)
{
    Turn *perst = calloc(1, sizeof(Turn));
    memcpy(perst, &turn, sizeof(Turn));
    if(!turns)
        return perst;
    lastTurn(turns)->next = perst;
    return turns;
}

#endif /* end of include guard: CONSSTATE_H */
