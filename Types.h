#ifndef TYPES_H
#define TYPES_H

typedef wchar_t wc;
typedef unsigned int uint;

typedef struct{
    wc *str;
    wc *pos;
    wc *end;
}Buffer;

typedef wc Board[8][8];
typedef bool Moves[8][8];

typedef enum{
    S_NEUTRAL, S_CHECK, S_MATE, S_N
}GameState;
const wc *GameStateStr[S_N] = {
    L"S_NEUTRAL", L"S_CHECK", L"S_MATE"
};

typedef enum{
    C_NONE, C_WHITE, C_BLACK, C_N
}Color;
const wc *ColorStr[C_N] = {
    L"C_NONE", L"C_WHITE", L"C_BLACK"
};

typedef enum{
    M_INVALID, M_HALF, M_VALID, M_CAPTURE, M_PROMOTE, M_CASTLE, M_PASSANT, M_N
}MoveType;
const wc *MoveTypeStr[M_N] = {
    L"M_INVALID", L"M_HALF", L"M_VALID", L"M_CAPTURE", L"M_PROMOTE", L"M_CASTLE", L"M_PASSANT"
};

typedef enum{
    A_INVALID, A_ADJ, A_DAG, A_N
}Algn;
const wc *AlgnStr[A_N] = {
    L"A_INVALID", L"A_ADJ", L"A_DAG"
};

typedef enum{
    D_U, D_R, D_D, D_L, D_N, D_INVALID
}Dir;
const wc *DirStr[D_N] = {
    L"D_U", L"D_R", L"D_D", L"D_L"
};

typedef enum{P_DEFAULT, P_COLORS, P_MOVES}PrintType;

uint minu(const uint a, const uint b)
{
    return a < b ? a : b;
}

uint maxu(const uint a, const uint b)
{
    return a > b ? a : b;
}

typedef union{
    uint arr[2];
    struct{
        uint x;
        uint y;
    };
    struct{
        uint col;
        uint row;
    };
    struct{
        uint w;
        uint h;
    };
}Pairu;

typedef struct{
    MoveType type;
    Pairu src;
    Pairu dst;
}Move;

typedef struct Turn{
    GameState state;
    Move move;
    struct Turn *next;
}Turn;

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

Color colorInv(const Color color)
{
    if(color == C_WHITE)
        return C_BLACK;
    if(color == C_BLACK)
        return C_WHITE;
    return C_NONE;
}

#endif /* end of include guard: TYPES_H */
