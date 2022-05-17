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

void boardCpy(Board dst, Board src)
{
    for(uint i = 0; i < 8; i++)
        memcpy(dst[i], src[i], sizeof(Piece)*8);
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

bool isEnPassant(Board board, Turn *turn)
{
    if(pieceAt(board, turn->dst.pos) != P_EMPTY)
        return false;
    if(
        (turn->src.piece == P_PAWN_B && turn->dst.piece == P_PAWN_B) ||
        (turn->src.piece == P_PAWN_W && turn->dst.piece == P_PAWN_W)
    ){
        return turn->src.pos.x != turn->dst.pos.x;
    }
    return false;
}

void applyPassant(Board board, Turn *turn)
{
    const Piece capPiece = turn->src.piece == P_PAWN_B ? P_PAWN_W : P_PAWN_B;
    const Dir bdir = turn->src.piece == P_PAWN_B ? D_U : D_D;
    if(pieceSet(board, shift(turn->dst.pos, bdir, 1), P_EMPTY) != capPiece){
        fwprintf(stderr, L"Error: enpassant error");
        exit(EXIT_FAILURE);
    }
}

bool isCastle(Turn *turn)
{
    return (
        turn->src.piece == P_KING_B && turn->dst.piece == P_ROOK_B
    ) || (
        turn->src.piece == P_KING_W && turn->dst.piece == P_ROOK_W
    );
}

/*
move king two squares toward rook.
move rook to square crossed by king.
*/
void applyCastle(Board board, Turn *turn)
{
    const Dir rookDir = turn->dst.pos.x == 0 ? D_L : D_R;
    pieceSet(board, shift(turn->src.pos, rookDir, 2), turn->src.piece);
    pieceSet(board, shift(turn->src.pos, rookDir, 1), turn->dst.piece);
    pieceSet(board, turn->dst.pos, P_EMPTY);
}

Turn *applyTurn(GameState state, Turn *turn)
{
    if(!turn)
        return NULL;
    setValidAt(statemoved, turn->src.pos, true, true);
    setValidAt(statemoved, turn->dst.pos, true, true);
    bool castle = false;
    if(isEnPassant(state.board, turn))
        applyPassant(state.board, turn);
    else if((castle = isCastle(turn)))
        applyCastle(state.board, turn);
    if(pieceSet(state.board, turn->src.pos, P_EMPTY) != turn->src.piece){
        fwprintf(
            stderr,
            L"Error: board piece: '%lc' doesn't match move piece: '%lc' at: %ls\n",
            pwc[pieceAt(state.board, turn->src.pos)],
            pwc[turn->src.piece],
            vec2Strify(turn->src.pos)
        );
        exit(EXIT_FAILURE);
    }
    if(!castle)
        pieceSet(state.board, turn->dst.pos, turn->dst.piece);

    state.playerTurn = colorInv(state.playerTurn);
    return turn->next;
}

GameState consGameState(Turn *turns)
{
    GameState ret = {.playerTurn = C_WHITE};
    resetBoard(ret.board);
    resetValid(ret.moved);
    while(turns)
        turns = applyTurn(board, moved, turns);
    applyTurn(board, moved, turns);
    return turns;
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

bool pawnDoubleMove(Turn *turn)
{
    if(!turn)
        return false;
    return
        (turn->src.piece == P_PAWN_B && turn->dst.piece == P_PAWN_B && turn->src.pos.y == 1 && turn->dst.pos.y == 3) ||
        (turn->src.piece == P_PAWN_W && turn->dst.piece == P_PAWN_W && turn->src.pos.y == 6 && turn->dst.pos.y == 4);
}

Vec2 getKing(Board board, const Color srcColor)
{
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: cannot get C_BLACK king\n");
        exit(EXIT_FAILURE);
    }
    const Piece srcPiece = srcColor == C_WHITE ? P_KING_W : P_KING_B;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec2 pos = {.x = x, .y = y};
            if(srcPiece == pieceAt(board, pos))
                return pos;
        }
    }
    fwprintf(stderr, L"Error: could not find player %ls's king\n", ColorStr[srcColor]);
    exit(EXIT_FAILURE);
    return (const Vec2){.x=-1, .y=-1};
}

bool inCheck(Board board, const Color srcColor)
{
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: player %ls cannot be in check\n", ColorStr[srcColor]);
        exit(EXIT_FAILURE);
    }
    const Vec2 king = getKing(board, srcColor);

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec2 src = {.x = x, .y = y};
            const Piece srcPiece = pieceAt(board, src);
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

bool movesCanEscapeCheck(Board board, Valid moves, const Vec2 src, const Color srcColor)
{
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec2 dst = {.x = x, .y = y};
            if(!getValidAt(moves, dst, true))
                continue;
            Turn turn = {.src.pos = src, .dst.pos = dst};
            Board applied;
            boardCpy(applied, board);
            applyTurn(applied, NULL, &turn, NULL);
            if(!inCheck(applied, srcColor))
                return true;
        }
    }
    return false;
}

bool inCheckMate(Turn *game, const Color srcColor)
{
    Board board;
    resetBoard(board);
    Valid moved;
    GameStateType type = G_NEUTRAL_W;
    consBoardState(board, moved, game, &type);
    if(!inCheck(board, srcColor))
        return false;

    Vec2 king;
    Vec2 rooks[2];
    if(srcColor == C_BLACK){
        king.x=4;
        king.y=0;
        rooks[0].x=0;
        rooks[0].y=0;
        rooks[1].x=7;
        rooks[1].y=0;
    }else if(C_WHITE){
        king.x=4;
        king.y=7;
        rooks[0].x=0;
        rooks[0].y=7;
        rooks[1].x=7;
        rooks[1].y=7;
    }else{
        fwprintf(stderr, L"Error: player %ls cannot be in mate\n", ColorStr[srcColor]);
        exit(EXIT_FAILURE);
    }

    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec2 src = {.x = x, .y = y};
            const Piece srcPiece = pieceAt(board, src);
            const Color srcColor = pieceColor(srcPiece);
            if(srcColor == C_NONE)
                continue;

            Valid moves;
            resetValid(moves);
            validMovesStateless(board, moves, src);
            if(movesCanEscapeCheck(board, moves, src, srcColor))
                return false;
        }
    }

    if(!turnPosChanged(game, king) && !turnPosChanged(game, rooks[0])){
        return true;
    }
    if(!turnPosChanged(game, king) && !turnPosChanged(game, rooks[1])){
        return true;
    }
    return true;
}

Turn* nextTurn(Turn *game, GameState state)
{
    if(state.type >= G_DRAW){
        fwprintf(
            stderr,
            L"Error: can't nextTurn() when GameState == %ls\n",
            GameStateTypeStr[state.type]
        );
        exit(EXIT_FAILURE);
    }

    Input in = {0};
    in.turn.src.piece = P_EMPTY;
    in.turn.dst.piece = P_EMPTY;
    Valid moves;
    do{
        clearTerm();
        BoardStr str;
        boardStrify(state.board, str);
        gameStatePrintPrompt(state.type);

        if(in.type == I_HALF){
            validMoves(game, moves, in.turn.src.pos);
            selectValid(str, in.turn.src.pos, moves, L"[]", L"><");
        }else if(in.type == I_FULL){
            boardStrSelect(str, in.turn.src.pos, L"[]");
            boardStrSelect(str, in.turn.dst.pos, L"><");
        }

        wprintf(L"%ls", str);
        in = getInput(in, state.board, state.type);
    }while(
        in.type != I_VALID ||
        !getValidAt(moves, in.turn.dst.pos, false)
    );
    Turn *turn = calloc(1, sizeof(Turn));
    *turn = in.turn;
    return game = appendTurn(game, turn);
}

GameStateType evalState(Turn *game)
{
    if(!game){
        fwprintf(stderr, L"Error: game should contain at least one turn\n");
        exit(EXIT_FAILURE);
    }

    Turn *last = lastTurn(game);
    if(pieceColor(last->src.piece) == C_WHITE)
        return G_NEUTRAL_B;
    return G_NEUTRAL_W;
}

GameState consGameState(Turn *game)
{
    GameState ret = {0};
    ret.type = G_NEUTRAL_W;
    ret.game = game;
    ret.last = consBoardState(ret.board, ret.moved, game, &ret.type);
    return ret;
}

#endif /* end of include guard: GAME_H */
