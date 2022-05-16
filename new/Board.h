#ifndef BOARD_H
#define BOARD_H

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

Piece boardAt(Board board, const Vec pos)
{
    return board[pos.y][pos.x];
}

Piece boardSet(Board board, const Vec pos, const Piece piece)
{
    const Piece oldPiece = boardAt(board, pos);
    board[pos.y][pos.x] = piece;
    return oldPiece;
}


Vec boardGet(Board board, const Color srcColor)
{
    if(srcColor == C_NONE){
        fwprintf(stderr, L"Error: cannot get C_BLACK king\n");
        exit(EXIT_FAILURE);
    }
    const Piece srcPiece = srcColor == C_WHITE ? P_KING_W : P_KING_B;
    for(int y = 0; y < 8; y++){
        for(int x = 0; x < 8; x++){
            const Vec pos = {.x = x, .y = y};
            if(srcPiece == boardAt(board, pos))
                return pos;
        }
    }
    fwprintf(stderr, L"Error: could not find player %ls's king\n", ColorStr[srcColor]);
    exit(EXIT_FAILURE);
    return (const Vec){.x=-1, .y=-1};
}

#endif /* end of include guard: BOARD_H */
