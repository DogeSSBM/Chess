#ifndef PIECE_H
#define PIECE_H

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

bool castleablePieces(const Piece p1, const Piece p2)
{
    return (
        p1 == P_KING_B && p2 == P_ROOK_B
    ) || (
        p1 == P_KING_W && p2 == P_ROOK_W
    ) || (
        p2 == P_KING_B && p1 == P_ROOK_B
    ) || (
        p2 == P_KING_W && p1 == P_ROOK_W
    );
}

#endif /* end of include guard: PIECE_H */
