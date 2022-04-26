#ifndef MOVECHECK_H
#define MOVECHECK_H

Pairu shift(const Pairu target, const Dir dir, const int dist)
{
    switch(dir){
        case D_U:
            return (const Pairu){.x = target.x, .y = target.y - dist};
            break;
        case D_R:
            return (const Pairu){.x = target.x + dist, .y = target.y};
            break;
        case D_D:
            return (const Pairu){.x = target.x, .y = target.y + dist};
            break;
        case D_L:
            return (const Pairu){.x = target.x - dist, .y = target.y};
            break;
        default:
            fwprintf(stderr, L"Error: invalid direction: %i\n", dir);
            exit(EXIT_FAILURE);
    }
}

Pairu shift2(const Pairu target, const Dir dir1, const Dir dir2, const int dist)
{
    return shift(shift(target, dir1, dist), dir2, dist);
}

Pairu shiftAlgn(const Pairu target, const Dir dir, const Algn algn, const int dist)
{
    if(algn == A_ADJ)
        return shift(target, dir, dist);
    return shift2(target, dir, dirRor(dir), dist);
}

MoveType getMoveAt(Moves moves, const Pairu target)
{
    if(!pairuInBounds(target)){
        fwprintf(
            stderr, L"Error: move target (%u, %u) out of bounds\n",
            target.x, target.y
        );
        exit(EXIT_FAILURE);
    }
    return moves[target.y][target.x];
}

void setMoveAt(Moves moves, const Pairu target, const MoveType mt)
{
    if(!pairuInBounds(target)){
        fwprintf(
            stderr, L"Error: move target (%u, %u) out of bounds\n",
            target.x, target.y
        );
        exit(EXIT_FAILURE);
    }
    moves[target.y][target.x] = mt;
}

uint validMoveCount(Moves moves)
{
    uint total = 0;
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Pairu cur = {.x = x, .y = y};
            total += getMoveAt(moves, cur) != M_INVALID;
        }
    }
    return total;
}

void resetMoves(Moves moves)
{
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Pairu cur = {.x = x, .y = y};
            setMoveAt(moves, cur, M_INVALID);
        }
    }
}

bool movable(Moves moves, const Pairu dst, const Color dstColor, const Color srcColor)
{
    if(!pairuInBounds(dst))
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

uint cast(Board board, Moves moves, const Pairu src, const Algn algn, const Dir dir, const uint dist)
{
    if(!pairuInBounds(src))
        return 0;
    const Color srcColor = pieceColor(getAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    uint dirDist = dist ? dist : 8;

    for(uint i = 1; i < dirDist; i++){
        const Pairu dst = shiftAlgn(src, dir, algn, i);
        if(
            !pairuInBounds(dst) ||
            !movable(moves, dst, pieceColor(getAt(board, dst)), srcColor)
        )
            return total;
        total++;
    }
    return total;
}

uint prop(Board board, Moves moves, const Pairu src, const Algn algn, const uint dist)
{
    uint total = 0;
    for(Dir i = D_U; i < D_N; i++)
        total += cast(board, moves, src, algn, i, dist);

    return total;
}

Turn* applyTurn(Board, Turn *); // i know... this is hax
bool hasMoved(const Pairu src, Turn *game)
{
    if(!game)
        return false;
    Board board;
    resetBoard(board);
    const wc piece = getAt(board, src);
    do{
        if(getAt(board, src) != piece)
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

uint knightMoves(Board board, Moves moves, const Pairu src)
{
    if(!pairuInBounds(src))
        return 0;

    fwprintf(
        stdout, L"src: (%u, %u)\n",
        src.x, src.y
    );
    const wc srcPiece = getAt(board, src);
    fwprintf(stdout, L"\nsrcPiece: %lc\n", srcPiece);
    const Color srcColor = pieceColor(srcPiece);

    uint total = 0;
    for(Dir i = 0; i < 4; i++){
        Pairu dst;
        Pairu fork = shift(src, i, 1);

        if(pairuInBounds(dst = shift(fork,dirRor(i),2)))
            total += movable(moves, dst, pieceColor(getAt(board,dst)), srcColor);
        if(pairuInBounds(dst = shift(fork,dirRol(i),2)))
            total += movable(moves, dst, pieceColor(getAt(board,dst)), srcColor);

        fork = shift(fork, i, 1);

        if(pairuInBounds(dst = shift(fork,dirRor(i),1)))
            total += movable(moves, dst, pieceColor(getAt(board,dst)), srcColor);
        if(pairuInBounds(dst = shift(fork,dirRol(i),1)))
            total += movable(moves, dst, pieceColor(getAt(board,dst)), srcColor);
    }
    return total;
}

Algn posAlgn(const Pairu src, const Pairu dst)
{
    if(eqPairu(src, dst))
        return A_INVALID;

    const Pairu dif = pairuAbsDif(src, dst);
    if(dif.x == dif.y)
        return A_DAG;

    if(minu(dif.x, dif.y) == 0)
        return A_ADJ;

    return A_INVALID;
}

uint findValidMoves(Board, Turn *, Moves, const Pairu); // moar hax
bool inCheck(Board board, const Color color)
{
    if(color == C_NONE)
        return false;
    const Pairu kingPos = getKing(board, color);
    const wc kingPiece = color == C_WHITE ? L'♚' : L'♔';
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Pairu pos = {.x=x, .y=y};
            const wc piece = getAt(board, pos);
            if(piece == L' ' || color == pieceColor(piece)|| piece == kingPiece)
                continue;
            Moves moves = {0};
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
void castlingMove(Board board, Turn *game, Moves moves, const Pairu src)
{
    const wc srcPiece = getAt(board, src);
    const Color srcColor = pieceColor(srcPiece);
    const Pairu dst = getKing(board, srcColor);
    const wc dstPiece = srcColor == C_WHITE ? L'♚' : L'♔';
    if(
        areCastilable(srcPiece, dstPiece) &&
        !hasCastled(game, srcColor) &&
        !hasMoved(src, game) &&
        !hasMoved(dst, game)
    ){
        const Dir dir = dst.x < src.x ? D_R : D_L;

        bool clear = true;
        for(Pairu step = shift(src, dir, 1); !eqPairu(dst, step); step = shift(step, dir, 1)){
            if(getAt(board, step) != L' '){
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

uint pawnMoves(Board board, Moves moves, const Pairu src)
{
    const wc srcPiece = getAt(board, src);
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
    const Pairu dst = shift(src, fDir, 1);
    if(!pairuInBounds(dst))
        return total;
    if(getAt(board, dst) == L' '){
        setMoveAt(moves, dst, M_VALID);
        total++;
        if(first){
            const Pairu fDst = shift(src, fDir, 2);
            if(pairuInBounds(fDst) && getAt(board, fDst) == L' '){
                setMoveAt(moves, fDst, M_VALID);
                total++;
            }
        }
    }

    Pairu cap = shift(dst, dirRol(fDir), 1);
    if(pairuInBounds(cap) && pieceColor(getAt(board, cap)) == colorInv(srcColor)){
        setMoveAt(moves, cap, M_CAPTURE);
        total++;
    }

    cap = shift(dst, dirRor(fDir), 1);
    if(pairuInBounds(cap) && pieceColor(getAt(board, cap)) == colorInv(srcColor)){
        setMoveAt(moves, cap, M_CAPTURE);
        total++;
    }

    return total;
}

Turn* lastTurn(Turn *); // yeah yeah, moar hax
void enPassant(Board board, Turn *game, Moves moves, const Pairu src)
{
    if(!game)
        return;

    const wc srcPiece = getAt(board, src);
    Turn *prvTurn = lastTurn(game);
    const Pairu prvDst = prvTurn->move.dst;
    const Pairu prvSrc = prvTurn->move.src;
    const wc prvPiece = getAt(board, prvDst);
    if(prvDst.x != prvSrc.x)
        return;
    Dir fDir = D_INVALID;
    if(
        srcPiece == L'♟' && src.y == 4 &&
        prvPiece == L'♙' && prvSrc.y == 6 &&
        prvDst.y == 4
    )
        fDir = D_D;
    else if(
        srcPiece == L'♙' && src.y == 3 &&
        prvPiece == L'♟' && prvSrc.y == 1 &&
        prvDst.y == 3
    )
        fDir = D_U;
    else
        return;

    Pairu target;
    if(src.x < 7 && eqPairu((target = shift2(src, fDir, D_R, 1)), prvDst))
        setMoveAt(moves, target, M_PASSANT);
    else if(src.x > 0 && eqPairu((target = shift2(src, fDir, D_L, 1)), prvDst))
        setMoveAt(moves, target, M_PASSANT);
}

uint findValidMoves(Board board, Turn *game, Moves moves, const Pairu src)
{
    resetMoves(moves);
    const wc srcPiece = getAt(board, src);
    switch(srcPiece){
        case L'♜':
        case L'♖':
            if(game)
                castlingMove(board, game, moves, src);
            prop(board, moves, src, A_ADJ, 0);
            break;
        case L'♞':
        case L'♘':
            knightMoves(board, moves, src);
            break;
        case L'♝':
        case L'♗':
            prop(board, moves, src, A_DAG, 0);
            break;
        case L'♛':
        case L'♕':
            prop(board, moves, src, A_DAG, 0);
            prop(board, moves, src, A_ADJ, 0);
            break;
        case L'♚':
        case L'♔':
            prop(board, moves, src, A_DAG, 1);
            prop(board, moves, src, A_ADJ, 1);
            break;
        case L'♟':
        case L'♙':
            if(game)
                enPassant(board, game, moves, src);
            pawnMoves(board, moves, src);
            break;
        case L' ':
        default:
            break;
    }

    return validMoveCount(moves);
}

#endif /* end of include guard: MOVECHECK_H */
