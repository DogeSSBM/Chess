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

MoveType getMoveAt(MoveType moves[8][8], const Pairu target)
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

void setMoveAt(MoveType moves[8][8], const Pairu target, const MoveType mt)
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

uint validMoveCount(MoveType moves[8][8])
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

void resetMoves(MoveType moves[8][8])
{
    for(uint y = 0; y < 8; y++){
        for(uint x = 0; x < 8; x++){
            const Pairu cur = {.x = x, .y = y};
            setMoveAt(moves, cur, M_INVALID);
        }
    }
}

bool movable(MoveType moves[8][8], const Pairu dst, const Color dstColor, const Color srcColor)
{
    if(!pairuInBounds(dst))
        return false;
    if(dstColor == C_NONE){
        setMoveAt(moves, dst, M_VALID);
        return true;
    }
    if(dstColor != srcColor){
        setMoveAt(moves, dst, M_CAPTURE);
        return true;
    }
    return false;
}

uint cast(wc board[8][8], MoveType moves[8][8], const Pairu src, const Algn algn, const Dir dir, const uint dist)
{
    if(!pairuInBounds(src))
        return 0;
    const Color srcColor = pieceColor(getAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    uint dirDist = dist;
    if(!dist){
        uint const *axis = dirAlgnX(dir) ? &src.x : &src.y;
        dirDist = dirPos(dir) ? 8 - *axis : *axis;
    }
    for(uint i = 1; i < dirDist; i++){
        const Pairu dst = shiftAlgn(src, dir, algn, i);
        if(!pairuInBounds(dst) || !movable(moves, dst, pieceColor(getAt(board, dst)), srcColor))
            return total;
        total++;
    }
    return total;
}

uint prop(wc board[8][8], MoveType moves[8][8], const Pairu src, const Algn algn, const uint dist)
{
    uint total = 0;
    for(Dir i = 0; i < 4; i++)
        total += cast(board, moves, src, algn, i, dist);

    return total;
}

uint knightMoves(wc board[8][8], MoveType moves[8][8], const Pairu src)
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

uint pawnMoves(wc board[8][8], MoveType moves[8][8], const Pairu src)
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
    const Pairu dst1 = shift(src, fDir, 1);
    //if(pairuInBounds(dst1) && )
    setMoveAt(moves, dst1, M_VALID);
    Pairu dst2;
    if(pieceColor(getAt(board, (dst2 = shift(dst1, dirRol(fDir), 1)))) == colorInv(srcColor)){
        setMoveAt(moves, dst2, M_CAPTURE);
        total++;
    }
    if(pieceColor(getAt(board, (dst2 = shift(dst1, dirRor(fDir), 1)))) == colorInv(srcColor)){
        setMoveAt(moves, dst2, M_CAPTURE);
        total++;
    }
    if(pieceColor(getAt(board, dst1)) == C_NONE){
        total++;
        setMoveAt(moves, dst1, M_VALID);
        if(first && pieceColor(getAt(board, (dst2 = shift(dst1, fDir, 1)))) == C_NONE){
            setMoveAt(moves, dst2, M_VALID);
            total++;
        }
    }
    return total;
}

uint findValidMoves(wc board[8][8], MoveType moves[8][8], const Pairu src)
{
    resetMoves(moves);
    switch(getAt(board, src)){
        case L'♜':
        case L'♖':
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
            pawnMoves(board, moves, src);
            break;
        case L' ':
        default:
            break;
    }

    return validMoveCount(moves);
}

#endif /* end of include guard: MOVECHECK_H */
