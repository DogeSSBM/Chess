#ifndef GAME_H
#define GAME_H

void resetBoard(Board board)
{
    const Board initialBoard = {
        {P_ROOK_B,P_KNIGHT_B,P_BISHOP_B,P_QUEEN_B,P_KING_B,P_BISHOP_B,P_KNIGHT_B,P_ROOK_B},
        {P_PAWN_B,P_PAWN_B,P_PAWN_B,P_PAWN_B,P_PAWN_B,P_PAWN_B,P_PAWN_B,P_PAWN_B},
        {P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY},
        {P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY},
        {P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY},
        {P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY,P_EMPTY},
        {P_PAWN_W,P_PAWN_W,P_PAWN_W,P_PAWN_W,P_PAWN_W,P_PAWN_W,P_PAWN_W,P_PAWN_W},
        {P_ROOK_W,P_KNIGHT_W,P_BISHOP_W,P_QUEEN_W,P_KING_W,P_BISHOP_W,P_KNIGHT_W,P_ROOK_W}
    };
    for(uint i = 0; i < 8; i++)
        memcpy(board[i], initialBoard[i], sizeof(Piece)*8);
}

Piece pieceAt(Board board, const Vec2 pos)
{
    return board[pos.y][pos.x];
}

Piece pieceSet(Board board, const Vec2 pos, const Piece piece)
{
    const Piece oldPiece = pieceAt(board, pos);
    board[pos.y][pos.x] = piece;
    return oldPiece;
}

bool validPiece(const Piece piece)
{
    return piece >= 0 && piece < P_N;
}

Color pieceColor(const Piece piece)
{
    if(!validPiece(piece)){
        fwprintf(
            stderr,
            L"Error: (ull)%ill is not a valid piece\n",
            (ull)piece
        );
        exit(EXIT_FAILURE);
    }
    if(piece == P_EMPTY)
        return C_NONE;
    return piece > P_KING_W ? C_BLACK : C_WHITE;
}

Turn *applyTurn(Board board, Turn *turn)
{
    if(!turn)
        return NULL;
    if(pieceSet(board, turn->move.src.pos, P_EMPTY) != turn->move.src.piece){
        fwprintf(
            stderr,
            L"Error: board piece: '%lc' doesn't match move piece: '%lc' at: %ls\n",
            pwc[pieceAt(board, turn->move.src.pos)],
            pwc[turn->move.src.piece],
            Vec2Strify(turn->move.src.pos)
        );
        exit(EXIT_FAILURE);
    }
    return turn->next;
}

void consBoardState(Board board, Turn *game)
{
    resetBoard(board);
    while(game)
        game = applyTurn(board, game);

}

Turn* appendTurn(Turn *game, Turn *turn)
{
    if(!game)
        return turn;
    if(!turn)
        return game;
    Turn *cur = game;
    while(cur->next)
        cur = cur->next;
    cur->next = turn;
    return game;
}

Turn* lastTurn(Turn *game)
{
    if(!game)
        return NULL;
    while(game->next)
        game = game->next;
    return game;
}



Turn* nextTurn(Turn *game, GameState state)
{
    if(state >= G_DRAW){
        fwprintf(
            stderr,
            L"Error: can't nextTurn() when GameState == %ls\n",
            GameStateStr[state]
        );
        exit(EXIT_FAILURE);
    }

    Turn *turn = calloc(1, sizeof(Turn));
    turn->move = getPlayerMove(game, state);

     return game = appendTurn(game, turn);
}

#endif /* end of include guard: GAME_H */
