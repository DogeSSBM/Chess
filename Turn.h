#ifndef TURN_H
#define TURN_H

// applys turn to board and returns next turn
Turn* applyTurn(Board board, Turn *turn)
{
    if(!turn)
        return NULL;
    const wc srcPiece = getAt(board, turn->move.src);
    const wc dstPiece = setAt(board, turn->move.dst, srcPiece);
    if(turn->move.type == M_CASTLE){
        setAt(board, turn->move.src, dstPiece);
    }else{
        setAt(board, turn->move.src, L' ');
    }
    return turn->next;
}

void printGame(Turn *game)
{
    if(!game)
        return;
    Board board;
    resetBoard(board);
    uint i = 0;
    do{
        wprintf(L"~~~~~~~~~~~~~~~~%3u~~~~~~~~~~~~~~~~\n", i);
        printBoardMove(board, game->move);
        i++;

    }while((game = applyTurn(board, game)));
    wprintf(L"~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~\n");
}

Turn* appendTurn(Turn *head, Turn *tail)
{
    if(!head)
        return tail;
    if(!tail)
        return head;
    Turn *turn = head;
    while(turn->next)
        turn = turn->next;
    turn->next = tail;
    return head;
}

Turn* lastTurn(Turn *game)
{
    if(!game)
        return NULL;
    while(game->next)
        game = game->next;
    return game;
}

Turn* prvTurn(Turn *game, Turn *cur)
{
    if(!game || !cur || game == cur)
        return NULL;
    while(game->next != cur){
        if(!game->next)
            return NULL;
        game = game->next;
    }
    return game;
}

void freeGame(Turn *game)
{
    while(game){
        Turn *next = game->next;
        free(game);
        game = next;
    }
}

// allocs a turn
Turn* makeTurn(Board board, const Move move)
{
    Turn *turn = calloc(1, sizeof(Turn));
    if(!turn){
        fwprintf(stderr, L"Error: could not calloc for next turn\n");
        exit(EXIT_FAILURE);
    }
    turn->state = S_NEUTRAL;
    turn->move = move;
    return turn;
}

Turn* makeMove(Board board, Turn *game, const Move move)
{
    const wc srcPiece = getAt(board, move.src);
    const wc dstPiece = getAt(board, move.dst);
    setAt(board, move.dst, srcPiece);
    setAt(board, move.src, pieceColor(srcPiece)==pieceColor(dstPiece)?dstPiece:L' ');
    const Pairu passant = {.x = move.dst.x, .y = move.src.y};
    wprintf(L"%ls: (%u, %u)\n", MoveTypeStr[move.type], passant.x, passant.y);
    if(move.type == M_PASSANT){
        setAt(board, passant, L' ');
        wprintf(L"yo\n");
    }
    return appendTurn(game, makeTurn(board, move));
}

#endif /* end of include guard: TURN_H */
