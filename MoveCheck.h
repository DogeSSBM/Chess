#ifndef MOVECHECK_H
#define MOVECHECK_H

Pairu shift(const Pairu target, const Dir dir, const int dist)
{
    if(dirAlgnX(dir))
        return (const Pairu){.x = target.x+dir==D_L?-dist:dist, .y = target.y};
    return (const Pairu){.x = target.x, .y = target.y+dir==D_U?-dist:dist};
}

Pairu shift2(const Pairu target, const Dir dir1, const Dir dir2, const int dist)
{
    return shift(shift(target, dir1, dist), dir2, dist);
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

uint propCard(wc board[8][8], MoveType moves[8][8], const Pairu src, const uint dist)
{
    const uint movesBefore = validMoveCount(moves);
    if(!pairuInBounds(src))
        return 0;
    const Color srcColor = pieceColor(getAt(board, src));
    if(srcColor == C_NONE)
        return 0;
    const uint limit = dist == 0 || dist >= 7 ? 7 : dist;
    for(Dir dir = 0; dir < 4; dir++){
        for(uint i = 1; i < limit; i++){
            const Pairu dst = shift(src, dir, i);
            if(!pairuInBounds(dst))
                break;
            const wc dstPiece = getAt(board, dst);
            const Color dstColor = pieceColor(dstPiece);
            if(dstColor == C_NONE){
                setMoveAt(moves, dst, M_VALID);
                continue;
            }
            if(dstColor == colorInv(srcColor))
                setMoveAt(moves, dst, M_CAPTURE);
            break;
        }
    }
    return validMoveCount(moves)-movesBefore;
}

uint propDiag(wc board[8][8], MoveType moves[8][8], const Pairu src, const uint dist)
{
    const uint movesBefore = validMoveCount(moves);
    if(!pairuInBounds(src))
        return 0;
    const Color srcColor = pieceColor(getAt(board, src));
    if(srcColor == C_NONE)
        return 0;
    const uint limit = dist == 0 || dist >= 7 ? 7 : dist;
    for(Dir dir = 0; dir < 4; dir++){
        for(uint i = 1; i < limit; i++){
            const Pairu dst = shift2(src, dir, dirRor(dir), i);
            if(!pairuInBounds(dst))
                break;
            const wc dstPiece = getAt(board, dst);
            const Color dstColor = pieceColor(dstPiece);
            if(dstColor == C_NONE){
                setMoveAt(moves, dst, M_VALID);
                continue;
            }
            if(dstColor == colorInv(srcColor))
                setMoveAt(moves, dst, M_CAPTURE);
            break;
        }
    }
    return validMoveCount(moves)-movesBefore;
}

uint findValidMoves(wc board[8][8], MoveType moves[8][8], const Pairu src)
{
    resetMoves(moves);
    switch(getAt(board, src)){
        case L'♜':
        case L'♖':
            propCard(board, moves, src, 0);
            break;
        case L'♞':
        case L'♘':
            // knightMoves(board, moves, src);
            break;
        case L'♝':
        case L'♗':
            propDiag(board, moves, src, 0);
            break;
        case L'♛':
        case L'♕':
            propDiag(board, moves, src, 0);
            propCard(board, moves, src, 0);
            break;
        case L'♚':
        case L'♔':
            propDiag(board, moves, src, 1);
            propCard(board, moves, src, 1);
            break;
        case L'♟':
        case L'♙':
            // pawnMoves(board, moves, src);
            break;
        case L' ':
        default:
            break;
    }
    return validMoveCount(moves);
}

#endif /* end of include guard: MOVECHECK_H */
