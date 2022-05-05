#ifndef MOVECHECK_H
#define MOVECHECK_H

bool validPos(const Vec2u pos)
{
    if(pos.x < 0 || pos.x >= 8 || pos.y < 0 || pos.y >= 8){
        fwprintf(stderr, L"Error: %ls out of bounds\n", Vec2Strify(pos));
        exit(EXIT_FAILURE);
    }
    return true;
}

bool getValidAt(Valid moves, const Vec2u pos)
{
    validPos(pos);
    return moves[y][x];
}

void setValidAt(Valid moves, const Vec2u pos, const bool type)
{
    validPos(pos);
    moves[y][x] = type;
}

void resetValid(Valid moves)
{
    for(uint i = 0; i < 8; i++)
        memset(moves[i], false, sizeof(bool)*8);
}

uint validCount(Valid moves)
{
    uint total = 0;
    for(uint y = 0; y < 8; y++)
        for(uint x = 0; x < 8; x++)
            total += getValidAt(moves, (const Vec2){.x = x, .y = y});

    return total;
}

uint cast(Board board, Valid moves, const Vec2 src, const Dir dir, const uint dist)
{
    if(!validPos(src))
        return 0;
    const Color srcColor = pieceColor(pieceAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    const uint dirDist = dist ? dist : 8;

    for(uint i = 1; i <= dirDist; i++){
        const Vec2 dst = shift(src, dir, i);

        if(!validPos(dst))
            return total;

        total++;
        setValidAt(moves, dst);

        if(colorInv(pieceColor(pieceAt(board, dst))) == srcColor)
            return total;
    }
    return total;
}

uint knightMoves(Board board, Valid moves, const Vec2 src)
{
    if(!validPos(src))
        return 0;

    const Piece srcPiece = pieceAt(board, src);
    if(srcPiece != P_KNIGHT_B && srcPiece != P_KNIGHT_W){
        fwprintf(
            stderr,
            L"Error: Piece at %ls is not %lc or %lc\n",
            Vec2Strify(pos),
            pwc[P_KNIGHT_B],
            pwc[P_KNIGHT_W]
        );
        exit(EXIT_FAILURE);
    }
    const Color srcColor = pieceColor(srcPiece);

    uint total = 0;
    for(Dir i = 0; i < 4; i++){
        Vec2 dst;
        Vec2 fork = shift(src, i, 1);

        if(validPos(dst = shift(fork,dirRoR(i),2)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);
        if(validPos(dst = shift(fork,dirRoL(i),2)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);

        fork = shift(fork, i, 1);

        if(validPos(dst = shift(fork,dirRoR(i),1)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);
        if(validPos(dst = shift(fork,dirRoL(i),1)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);
    }
    return total;
}

Algn posAlgn(const Vec2 src, const Vec2 dst)
{
    if(eqVec2u(src, dst))
        return A_INVALID;

    const Vec2 dif = Vec2uAbsDif(src, dst);
    if(dif.x == dif.y)
        return A_DAG;

    if(minu(dif.x, dif.y) == 0)
        return A_ADJ;

    return A_INVALID;
}

uint findMovesMoves(Board, Turn *, Moves, const Vec2u); // moar hax
bool inCheck(Board board, const Color color)
{
    if(color == C_NONE)
        return false;
    const Vec2 kingPos = getKing(board, color);
    const wc kingPiece = color == C_WHITE ? L'♚' : L'♔';
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Vec2 pos = {.x=x, .y=y};
            const wc piece = pieceAt(board, pos);
            if(piece == L' ' || color == pieceColor(piece)|| piece == kingPiece)
                continue;
            Moves moves = {0};
            findMovesMoves(board, NULL, moves, pos);
            if(getValidAt(moves, kingPos) == M_CAPTURE)
                return true;
        }
    }
    return false;
}

bool areCastilable(const wc p1, const wc p2)
{
    return  (p1 != p2) &&
            (pieceColor(p1) == pieceColor(p2)) &&
            (p1 == L'♜' || p1 == L'♖' || p2 == L'♜' || p2 == L'♖') &&
            (p1 == L'♚' || p1 == L'♔' || p2 == L'♚' || p2 == L'♔');
}

// src < dst < src
//  ♖ L  ♔  R  ♖
void castlingMove(Board board, Turn *game, Moves moves, const Vec2 src)
{
    const Piece srcPiece = pieceAt(board, src);
    const Color srcColor = pieceColor(srcPiece);
    const Vec2 dst = getKing(board, srcColor);
    const Piece dstPiece = srcColor == C_WHITE ? L'♚' : L'♔';
    if(
        areCastilable(srcPiece, dstPiece) &&
        !hasCastled(game, srcColor) &&
        !hasMoved(src, game) &&
        !hasMoved(dst, game)
    ){
        const Dir dir = dst.x < src.x ? D_R : D_L;

        bool clear = true;
        for(Vec2 step = shift(src, dir, 1); !eqVec2u(dst, step); step = shift(step, dir, 1)){
            if(pieceAt(board, step) != L' '){
                clear = false;
                break;
            }
            Board stepState;
            cpyBoard(stepState, board);
            setAt(stepState, src, L' ');
            setAt(stepState, step, dstPiece);
        }
        if(clear)
            setValidAt(moves, dst, M_CASTLE);
    }
}

uint pawnMoves(Board board, Moves moves, const Vec2 src)
{
    const Piece srcPiece = pieceAt(board, src);
    const Color srcColor = pieceColor(srcPiece);
    bool first;
    Dir fDir;
    if(srcPiece == L'♟'){
        fDir = D_D;
        first = src.y == 1;
    }else if(srcPiece == L'♙'){
        fDir = D_U;
        first = src.y == 6;
    }else{
        fwprintf(
            stderr,
            L"Error: Piece at %ls is not %lc or %lc\n",
            Vec2Strify(pos),
            pwc[P_PAWN_B],
            pwc[P_PAWN_W]
        );
        exit(EXIT_FAILURE);
    }

    uint total = 0;
    const Vec2 dst = shift(src, fDir, 1);
    if(!validPos(dst))
        return total;
    if(pieceAt(board, dst) == L' '){
        setValidAt(moves, dst, M_VALID);
        total++;
        if(first){
            const Vec2 fDst = shift(src, fDir, 2);
            if(validPos(fDst) && pieceAt(board, fDst) == L' '){
                setValidAt(moves, fDst, M_VALID);
                total++;
            }
        }
    }

    Vec2 cap = shift(dst, dirRoL(fDir), 1);
    if(validPos(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, M_CAPTURE);
        total++;
    }

    cap = shift(dst, dirRoR(fDir), 1);
    if(validPos(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, M_CAPTURE);
        total++;
    }

    return total;
}

bool canPassant(Turn *game)
{
    if(!game)
        return false;
    game = lastTurn(game);
    const Piece srcPiece = game->move.src.piece;
    if(srcPiece != P_PAWN_B || srcPiece != P_PAWN_W)
        return false;
    const Color srcColor = pieceColor(srcPiece);
    if(srcColor == C_WHITE && game->move.src.pos.y == 1 && game->move.dst.pos.y == 3)
        return true;
    if(srcColor == C_BLACK && game->move.src.pos.y == 6 && game->move.dst.pos.y == 4)
        return true;
    return false;
}

uint pawnMoves(Board board, Moves moves, const Vec2 pos)
{
    return 0;
}

uint validMoves(Board board, Moves moves, Turn *game, const Vec2 pos)
{
    resetMoves(moves);
    const Piece srcPiece = pieceAt(board, pos);
    const Color srcColor = pieceColor(srcPiece);
    if(srcColor == C_NONE)
        return 0;
    switch(srcPiece){
        case P_PAWN_B:
        case P_PAWN_W:

            break;
        case P_ROOK_B:
        case P_ROOK_W:

            break;
        case P_KNIGHT_B:
        case P_KNIGHT_W:

            break;
        case P_BISHOP_B:
        case P_BISHOP_W:

            break;
        case P_QUEEN_B:
        case P_QUEEN_W:

            break;
        case P_KING_B:
        case P_KING_W:

            break;
        case P_EMPTY:
        default:
            return 0;
    }
    return 0;
}

Turn* turnPosChanged(Turn *game, const Vec2 pos)
{
    while(game){
        if(eqVec2(game->move.src.pos, pos) || eqVec2(game->move.dst.pos, pos))
            return game;
    }
    return NULL;
}

Move getPlayerMove(Turn *last)

#endif /* end of include guard: MOVECHECK_H
