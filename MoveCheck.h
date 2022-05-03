#ifndef MOVECHECK_H
#define MOVECHECK_H

bool validPos(const Vec2u pos)
{
    return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

MoveType getMoveAt(Valid moves, const Vec2u pos)
{
    if(!validPos(pos)){
        fwprintf(stderr, L"Error: %ls out of bounds\n", Vec2Strify(pos));
        return EXIT_FAILURE;
    }
    return moves[y][x];
}

void setMoveAt(Valid moves, const Vec2u pos, const MoveType type)
{
    moves[y][x] = type;
}

void resetMoves(Valid moves)
{
    for(uint i = 0; i < 8; i++)
        memset(moves[i], false, sizeof(bool)*8);
}

uint validMoveCount(Valid moves)
{
    uint total = 0;
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Vec2 cur = {.x = x, .y = y};
            total += getMoveAt(moves, cur) != M_INVALID;
        }
    }
    return total;
}

void resetMoves(Valid moves)
{
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Vec2 cur = {.x = x, .y = y};
            setMoveAt(moves, cur, M_INVALID);
        }
    }
}

bool movable(Valid moves, const Vec2 dst, const Color dstColor, const Color srcColor)
{
    if(!Vec2uInBounds(dst))
        return false;
    if(dstColor == C_NONE){
        setMoveAt(moves, dst, M_VALID);
        return true;
    }
    if(dstColor != srcColor){
        setMoveAt(moves, dst, M_CAPTURE);
        // return true;
    }
    return false;
}

uint cast(Board board, Valid moves, const Vec2 src, const Algn algn, const Dir dir, const uint dist)
{
    if(!Vec2uInBounds(src))
        return 0;
    const Color srcColor = pieceColor(pieceAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    uint dirDist = dist ? dist : 8;

    for(uint i = 1; i < dirDist; i++){
        const Vec2 dst = shiftAlgn(src, dir, algn, i);
        if(
            !Vec2uInBounds(dst) ||
            !movable(moves, dst, pieceColor(pieceAt(board, dst)), srcColor)
        )
            return total;
        total++;
    }
    return total;
}

uint prop(Board board, Valid moves, const Vec2 src, const Algn algn, const uint dist)
{
    uint total = 0;
    for(Dir i = D_U; i < D_N; i++)
        total += cast(board, moves, src, algn, i, dist);

    return total;
}

Turn* applyTurn(Board, Turn *); // i know... this is hax
bool hasMoved(const Vec2 src, Turn *game)
{
    if(!game)
        return false;
    Board board;
    resetBoard(board);
    const wc piece = pieceAt(board, src);
    do{
        if(pieceAt(board, src) != piece)
            return true;
    }while((game = applyTurn(board, game)));
    return false;
}

bool hasCastled(Turn *game, const Color color)
{
    if(!game)
        return false;
    Color prvColor = C_WHITE;
    do{
        if(game->move.type == M_CASTLE && color == prvColor)
            return true;
        prvColor = colorInv(prvColor);
    }while((game = game->next));
    return false;
}

uint knightMoves(Board board, Valid moves, const Vec2 src)
{
    if(!Vec2uInBounds(src))
        return 0;

    fwprintf(
        stdout, L"src: (%u, %u)\n",
        src.x, src.y
    );
    const Piece srcPiece = pieceAt(board, src);
    fwprintf(stdout, L"\nsrcPiece: %lc\n", srcPiece);
    const Color srcColor = pieceColor(srcPiece);

    uint total = 0;
    for(Dir i = 0; i < 4; i++){
        Vec2 dst;
        Vec2 fork = shift(src, i, 1);

        if(Vec2uInBounds(dst = shift(fork,dirRor(i),2)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);
        if(Vec2uInBounds(dst = shift(fork,dirRol(i),2)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);

        fork = shift(fork, i, 1);

        if(Vec2uInBounds(dst = shift(fork,dirRor(i),1)))
            total += movable(moves, dst, pieceColor(pieceAt(board,dst)), srcColor);
        if(Vec2uInBounds(dst = shift(fork,dirRol(i),1)))
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

uint findValidMoves(Board, Turn *, Moves, const Vec2u); // moar hax
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
            Valid moves = {0};
            findValidMoves(board, NULL, moves, pos);
            if(getMoveAt(moves, kingPos) == M_CAPTURE)
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
void castlingMove(Board board, Turn *game, Valid moves, const Vec2 src)
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
            setMoveAt(moves, dst, M_CASTLE);
    }
}

uint pawnMoves(Board board, Valid moves, const Vec2 src)
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
        return 0;
    }

    uint total = 0;
    const Vec2 dst = shift(src, fDir, 1);
    if(!Vec2uInBounds(dst))
        return total;
    if(pieceAt(board, dst) == L' '){
        setMoveAt(moves, dst, M_VALID);
        total++;
        if(first){
            const Vec2 fDst = shift(src, fDir, 2);
            if(Vec2uInBounds(fDst) && pieceAt(board, fDst) == L' '){
                setMoveAt(moves, fDst, M_VALID);
                total++;
            }
        }
    }

    Vec2 cap = shift(dst, dirRol(fDir), 1);
    if(Vec2uInBounds(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setMoveAt(moves, cap, M_CAPTURE);
        total++;
    }

    cap = shift(dst, dirRor(fDir), 1);
    if(Vec2uInBounds(cap) && pieceColor(pieceAt(board, cap)) == colorInv(srcColor)){
        setMoveAt(moves, cap, M_CAPTURE);
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

uint pawnMoves(Board board, Valid moves, const Vec2 pos)
{
    return 0;
}

uint validMoves(Board board, Valid moves, Turn *game, const Vec2 pos)
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

bool inCheck(Board board, const Color color)
{
    return false;
}

#endif /* end of include guard: MOVECHECK_H */
