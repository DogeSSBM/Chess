#ifndef MOVECHECK_H
#define MOVECHECK_H

uint cast(Board board, Valid moves, const Vec src, const Dir dir, const uint dist)
{
    if(!vecValid(src, true))
        return 0;

    const Color srcColor = pieceColor(boardAt(board, src));
    if(srcColor == C_NONE)
        return 0;

    uint total = 0;
    const uint dirDist = dist ? dist : 8;

    for(uint i = 1; i <= dirDist; i++){
        const Vec dst = shift(src, dir, i);

        if(!vecValid(dst, false))
            return total;

        if(pieceColor(boardAt(board, dst)) == srcColor)
            return total;

        total++;
        setValidAt(moves, dst, true, true);

        if(colorInv(pieceColor(boardAt(board, dst))) == srcColor)
            return total;
    }
    return total;
}

uint knightMoves(Board board, Valid moves, const Vec src)
{
    if(!vecValid(src, true))
        return 0;

    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_KNIGHT_B && srcPiece != P_KNIGHT_W){
        fwprintf(stderr, L"Error: cannot get knightMoves for non knight at %ls\n", vecStrify(src));
        exit(EXIT_FAILURE);
    }
    const Color srcColor = pieceColor(srcPiece);

    uint total = 0;
    Dir d = D_U;
    for(uint i = 0; i < 4; i++){
        Vec dst;
        Vec fork = shift(src, d, 1);

        if(vecValid(dst = shift(fork,dirRoR(d),2), false)){
            if(pieceColor(boardAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
        if(vecValid(dst = shift(fork,dirRoL(d),2), false)){
            if(pieceColor(boardAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }

        fork = shift(src, d, 2);

        if(vecValid(dst = shift(fork,dirRoR(d),1), false)){
            if(pieceColor(boardAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
        if(vecValid(dst = shift(fork,dirRoL(d),1), false)){
            if(pieceColor(boardAt(board,dst)) != srcColor){
                setValidAt(moves, dst, true, true);
                total++;
            }
        }
        d = dirRoR(d);
    }
    return total;
}

uint rookMoves(Board board, Valid moves, const Vec src)
{
    uint total = 0;
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_ROOK_W && srcPiece != P_ROOK_B){
        fwprintf(stderr, L"Error: cannot get rookMoves for non rook at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    Dir d = D_U;
    for(uint i = 0; i < 4; i++){
        total += cast(board, moves, src, d, 0);
        d = dirRoR(d);
    }
    return total;
}

uint bishopMoves(Board board, Valid moves, const Vec src)
{
    uint total = 0;
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_BISHOP_W && srcPiece != P_BISHOP_B){
        fwprintf(stderr, L"Error: cannot get bishopMoves for non bishop at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    Dir d = D_UR;
    for(uint i = 0; i < 4; i++){
        total += cast(board, moves, src, d, 0);
        d = dirRoR(d);
    }
    return total;
}

uint queenMoves(Board board, Valid moves, const Vec src)
{
    uint total = 0;
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_QUEEN_W && srcPiece != P_QUEEN_B){
        fwprintf(stderr, L"Error: cannot get queenMoves for non queen at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    Dir d = D_U;
    for(uint i = 0; i < 4; i++){
        total += cast(board, moves, src, d, 0) + cast(board, moves, src, dirRor(d), 0);
        d = dirRoR(d);
    }
    return total;
}

uint kingMoves(Board board, Valid moves, const Vec src)
{
    uint total = 0;
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_KING_W && srcPiece != P_KING_B){
        fwprintf(stderr, L"Error: cannot get kingMoves for non king at %ls\n", src);
        exit(EXIT_FAILURE);
    }

    Dir d = D_U;
    for(uint i = 0; i < 4; i++){
        total += cast(board, moves, src, d, 1) + cast(board, moves, src, dirRor(d), 1);
        d = dirRoR(d);
    }
    return total;
}

uint pawnMoves(Board board, Valid moves, const Vec src)
{
    const Piece srcPiece = boardAt(board, src);
    const Color srcColor = pieceColor(srcPiece);
    bool first;
    Dir fDir;
    if(srcPiece == P_PAWN_W){
        fDir = D_U;
        first = src.y == 6;
    }else if(srcPiece == P_PAWN_B){
        fDir = D_D;
        first = src.y == 1;
    }else{
        fwprintf(stderr, L"Error: cannot get pawnMoves for non pawn at %ls\n", vecStrify(src));
        exit(EXIT_FAILURE);
    }

    uint total = 0;
    const Vec dst = shift(src, fDir, 1);
    if(!vecValid(dst, false))
        return total;
    if(boardAt(board, dst) == P_EMPTY){
        setValidAt(moves, dst, true, true);
        total++;
        if(first){
            const Vec fDst = shift(src, fDir, 2);
            if(vecValid(fDst, false) && boardAt(board, fDst) == P_EMPTY){
                setValidAt(moves, fDst, true, true);
                total++;
            }
        }
    }

    Vec cap = shift(dst, dirRoL(fDir), 1);
    if(vecValid(cap, false) && pieceColor(boardAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, true, true);
        total++;
    }

    cap = shift(dst, dirRoR(fDir), 1);
    if(vecValid(cap, false) && pieceColor(boardAt(board, cap)) == colorInv(srcColor)){
        setValidAt(moves, cap, true, true);
        total++;
    }

    return total;
}

uint validMovesStateless(Board board, Valid moves, const Vec pos)
{
    resetValid(moves);
    const Piece srcPiece = boardAt(board, pos);
    const Color srcColor = pieceColor(srcPiece);
    if(srcColor == C_NONE){
        return 0;
        fwprintf(stderr, L"Error: cannot get validMoves for blank square\n");
        exit(EXIT_FAILURE);
    }

    uint total = 0;
    switch(srcPiece){
        case P_PAWN_B:
        case P_PAWN_W:
            total += pawnMoves(board, moves, pos);
            break;
        case P_ROOK_B:
        case P_ROOK_W:
            total += rookMoves(board, moves, pos);
            break;
        case P_KNIGHT_B:
        case P_KNIGHT_W:
            total += knightMoves(board, moves, pos);
            break;
        case P_BISHOP_B:
        case P_BISHOP_W:
            total += bishopMoves(board, moves, pos);
            break;
        case P_QUEEN_B:
        case P_QUEEN_W:
            total += queenMoves(board, moves, pos);
            break;
        case P_KING_B:
        case P_KING_W:
            total += kingMoves(board, moves, pos);
            break;
        case P_EMPTY:
        default:
            fwprintf(stderr, L"Error: default\n");
            exit(EXIT_FAILURE);
            break;
    }

    return 0;
}

bool pawnDoubleMove(Turn *turn)
{
    if(!turn)
        return false;
    return
        (turn->src.piece == P_PAWN_B && turn->dst.piece == P_PAWN_B && turn->src.pos.y == 1 && turn->dst.pos.y == 3) ||
        (turn->src.piece == P_PAWN_W && turn->dst.piece == P_PAWN_W && turn->src.pos.y == 6 && turn->dst.pos.y == 4);
}

uint passantMoves(Board board, Valid moves, const Vec src, Turn *last)
{
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_PAWN_B && srcPiece != P_PAWN_W){
        fwprintf(stderr, L"Error: cannot get passantMoves for non pawn at %ls\n", vecStrify(src));
        exit(EXIT_FAILURE);
    }
    if(!pawnDoubleMove(last))
        return 0;
    Vec temp;
    const Dir fdir = srcPiece == P_PAWN_B ? D_D : D_U;
    if(vecEq((temp = shift(src, D_L, 1)), last->dst.pos)){
        setValidAt(moves, shift(temp, fdir, 1), true, true);
        return 1;
    }
    if(vecEq((temp = shift(src, D_R, 1)), last->dst.pos)){
        setValidAt(moves, shift(temp, fdir, 1), true, true);
        return 1;
    }
    return 0;
}

uint castleMoves(Board board, Valid moves, Valid moved, const Vec src)
{
    const Piece srcPiece = boardAt(board, src);
    if(srcPiece != P_KING_B && srcPiece != P_KING_W){
        fwprintf(stderr, L"Error: cannot get castleMoves for non king at %ls\n", vecStrify(src));
        exit(EXIT_FAILURE);
    }

    Piece rookPiece = srcPiece == P_KING_B ? P_ROOK_B : P_ROOK_W;
    Vec r[2] = {
        (Vec){.x = 0, .y = srcPiece == P_KING_B ? 0 : 7},
        (Vec){.x = 7, .y = srcPiece == P_KING_B ? 0 : 7}
    };

    const Color srcColor = pieceColor(srcPiece);
    if(inCheck(board, srcColor))
        return 0;

    uint total = 0;
    const Dir d[2] = {D_L, D_R};
    for(uint i = 0; i < 2; i++){
        if(getValidAt(moved, r[i], true))
            continue;
        Board test;
        boardCpy(test, board);
        boardSet(test, src, P_EMPTY);
        if(boardSet(test, shift(src, d[i], 1), srcPiece) != P_EMPTY || inCheck(test, srcColor))
            continue;
        boardSet(test, shift(src, d[i], i), P_EMPTY);
        if(boardSet(test, shift(src, d[i], 2), srcPiece) != P_EMPTY || inCheck(test, srcColor))
            continue;
        Piece testpiece;
        for(
            Vec testpos = shift(r[i], dirInv(d[i]), 1);
            (testpiece = boardAt(board, testpos)) != srcPiece;
            testpos = shift(testpos, dirInv(d[i]), 1)
        ){
            if(testpiece != P_EMPTY)
                goto skip;
        }
        boardSet(test, r[i], P_EMPTY);
        boardSet(test, shift(src, d[i], 1), rookPiece);
        if(!inCheck(test, srcColor)){
            setValidAt(moves, r[i], true, true);
            total++;
        }
        skip:
        ;
    }
    return total;
}

int invalidateCheckMoves(Board board, Valid moves, const Vec src)
{
    int invalid = 0;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec dst = {.x = x, .y = y};
            if(!getValidAt(moves, dst, true))
                continue;
            const Turn turn = {
                .src = {
                    .piece = boardAt(board, src),
                    .pos = src
                },
                .dst = {
                    .piece = boardAt(board, dst),
                    .pos = dst
                }
            };
            Board after;
            boardCpy(after, board);
            applyMove(after, turn);
            if(inCheck(board, pieceColor(turn.src.piece))){
                setValidAt(moves, dst, false, true);
                invalid++;
            }
        }
    }
    return invalid;
}

uint validMoves(GameState state, Valid moves, const Vec src)
{
    resetValid(moves);
    uint total = 0;
    const Piece srcPiece = boardAt(state.board, src);

    if(srcPiece == P_PAWN_B || srcPiece == P_PAWN_W)
        total += passantMoves(state.board, moves, src, state.last);

    if(srcPiece == P_KING_B || srcPiece == P_KING_W)
        total += castleMoves(state.board, moves, state.moved, src);

    total += validMovesStateless(state.board, moves, src);

    return total;
}

void validAllReset(AllValid all)
{
    for(int y = 0; y < 8; y++)
        for(int x = 0; x < 8; x++)
            resetValid(all[x][y]);
}

uint validAllMoves(GameState state, AllValid all)
{
    uint total = 0;
    validAllReset(all);
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec src = {.x = x, .y = y};
            const Color srcColor = pieceColor(boardAt(state.board, src));
            if(
                (state.playerTurn == C_NONE && srcColor != state.playerTurn) ||
                (state.playerTurn != C_NONE && srcColor == state.playerTurn)
            ){
                total += validMoves(state, all[x][y], src);
                total -= invalidateCheckMoves(state.board, all[x][y], src);
            }
        }
    }
    return total;
}

#endif /* end of include guard: MOVECHECK_H */
