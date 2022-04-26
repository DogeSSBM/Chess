#ifndef TYPES_H
#define TYPES_H

typedef wchar_t wc;
typedef unsigned int uint;

typedef wc Board[8][8];
typedef bool Moves[8][8];

typedef enum{S_NEUTRAL, S_CHECK, S_MATE}GameState;
typedef enum{C_NONE, C_WHITE, C_BLACK}Color;
typedef enum{M_INVALID, M_HALF, M_VALID, M_CAPTURE, M_PROMOTE, M_CASTLE, M_PASSANT}MoveType;
const wc *MoveTypeStr[7] = {L"M_INVALID", L"M_HALF", L"M_VALID", L"M_CAPTURE", L"M_PROMOTE", L"M_CASTLE", L"M_PASSANT"};
typedef enum{A_INVALID, A_ADJ, A_DAG}Algn;
typedef enum{D_U, D_R, D_D, D_L, D_N, D_INVALID}Dir;
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

Pairu pairuAbsDif(const Pairu p1, const Pairu p2)
{
    return (const Pairu){
        .x = maxu(p1.x, p2.x) - minu(p1.x, p2.x),
        .y = maxu(p1.y, p2.y) - minu(p1.y, p2.y)
    };
}

bool eqPairu(const Pairu p1, const Pairu p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}

bool dirAlgnX(const Dir dir)
{
    return dir == D_L || dir == D_R;
}

bool dirAlgnY(const Dir dir)
{
    return dir == D_U || dir == D_D;
}

bool dirPos(const Dir dir)
{
    return dir == D_D || dir == D_R;
}

bool dirNeg(const Dir dir)
{
    return dir == D_U || dir == D_L;
}

Dir dirInv(const Dir dir)
{
    return dir^(1u<<1);
}

Dir dirRor(const Dir dir)
{
    return (dir+1)%4;
}

Dir dirRol(const Dir dir)
{
    return (dir+3)%4;
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
