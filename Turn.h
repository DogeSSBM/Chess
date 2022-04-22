#ifndef TURN_H
#define TURN_H

Turn* appendTurn(Turn *head, Turn *tail)
{
    if(!head)
        return tail;
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

void freeGame(Turn *game)
{
    while(game){
        Turn *next = game->next;
        free(game);
        game = next;
    }
}

Turn* makeTurn(Board board, Move move)
{
    Turn *turn = calloc(1, sizeof(turn));
    if(!turn){
        fwprintf(stderr, L"Error: could not calloc for next turn\n");
        exit(EXIT_FAILURE);
    }
    turn->state = S_NEUTRAL;
    turn->move = move;
    return turn;
}

#endif /* end of include guard: TURN_H */
