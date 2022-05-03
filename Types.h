#ifndef TYPES_H
#define TYPES_H

typedef wchar_t wc;
typedef unsigned int uint;
typedef unsigned long long ull;

typedef union{
    int arr[2];
    struct{
        int x;
        int y;
    };
    struct{
        int col;
        int row;
    };
    struct{
        int file;
        int rank;
    };
}Vec2;

typedef enum{
    P_PAWN_W, P_ROOK_W, P_KNIGHT_W, P_BISHOP_W, P_QUEEN_W, P_KING_W,
    P_PAWN_B, P_ROOK_B, P_KNIGHT_B, P_BISHOP_B, P_QUEEN_B, P_KING_B,
    P_EMPTY, P_N
}Piece;

typedef Piece Board[8][8];
typedef wc BoardStr[708];

typedef  Valid[8][8];

const static wc pwc[P_N] = {
    L'♟', L'♜', L'♞', L'♝', L'♛', L'♚',
    L'♙', L'♖', L'♘', L'♗', L'♕', L'♔',
    L' '
};

const static wc pch[P_N] = {
    L'P', L'R', L'K', L'B', L'Q', L'K',
    L'p', L'r', L'k', L'b', L'q', L'k',
    L' '
};

typedef enum{
    C_NONE, C_WHITE, C_BLACK, C_N
}Color;

const wc *ColorStr[C_N] = {
    L"C_NONE", L"C_WHITE", L"C_BLACK"
};

typedef enum{
    D_U, D_UR, D_R, D_DR,
    D_D, D_DL, D_L, D_UL,
    D_N
}Dir;

const wc *DirStr[D_N] = {
    L"D_U", L"D_UR", L"D_R", L"D_DR",
    L"D_D", L"D_DL", L"D_L", L"D_UL"
};

typedef struct{
    struct{
        Piece piece;
        Vec2 pos;
    }src;
    struct{
        Piece piece;
        Vec2 pos;
    }dst;
}Move;

typedef struct Turn{
    Move move;
    struct Turn *next;
}Turn;

Color colorInv(const Color color)
{
    if(color == C_WHITE)
        return C_BLACK;
    if(color == C_BLACK)
        return C_WHITE;
    return C_NONE;
}

uint maxu(const uint a, const uint b)
{
    return a>b?a:b;
}

uint minu(const uint a, const uint b)
{
    return a<b?a:b;
}


#endif /* end of include guard: TYPES_H */
