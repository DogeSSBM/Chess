#ifndef TYPES_H
#define TYPES_H

#define BOARDSTRLEN 709

typedef wchar_t wc;
typedef unsigned int uint;
typedef unsigned long long ull;

typedef enum{
    D_U, D_UR, D_R, D_DR,
    D_D, D_DL, D_L, D_UL,
    D_N
}Dir;

typedef union{
    int arr[2];
    struct{
        int x;
        int y;
    };
    struct{
        Dir dir;
        int len;
    };
}Vec;

typedef enum{
    P_PAWN_W, P_ROOK_W, P_KNIGHT_W, P_BISHOP_W, P_QUEEN_W, P_KING_W,
    P_PAWN_B, P_ROOK_B, P_KNIGHT_B, P_BISHOP_B, P_QUEEN_B, P_KING_B,
    P_EMPTY, P_N
}Piece;

typedef Piece Board[8][8];
typedef wc BoardStr[BOARDSTRLEN];

typedef bool Valid[8][8];

const wc pwc[P_N] = {
    L'♟', L'♜', L'♞', L'♝', L'♛', L'♚',
    L'♙', L'♖', L'♘', L'♗', L'♕', L'♔',
    L' '
};

const wc pch[P_N] = {
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

typedef enum{G_NEUTRAL_W, G_NEUTRAL_B, G_CHECK_W, G_CHECK_B, G_DRAW, G_MATE_W, G_MATE_B, G_N}GameStateType;
const wc *GameStateTypeStr[G_N] = {
    L"G_NEUTRAL_W", L"G_NEUTRAL_B", L"G_CHECK_W", L"G_CHECK_B", L"G_DRAW", L"G_MATE_W", L"G_MATE_B"
};

const wc *DirStr[D_N] = {
    L"D_U", L"D_UR", L"D_R", L"D_DR",
    L"D_D", L"D_DL", L"D_L", L"D_UL"
};

typedef struct Turn{
    struct{
        Piece piece;
        Vec pos;
    }src;
    struct{
        Piece piece;
        Vec pos;
    }dst;
    struct Turn *next;
}Turn;

typedef struct{
    Color playerTurn;
    Board board;
    Valid moved;
    Turn *last;
}GameState;

typedef struct{
    char str[512];
}Input;

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
