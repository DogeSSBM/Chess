#ifndef TYPES_H
#define TYPES_H

typedef wchar_t wc;
typedef unsigned int uint;

typedef enum{S_NEUTRAL, S_CHECK_W, S_CHECK_B, S_MATE_W, S_MATE_B}GameState;
typedef enum{C_NONE, C_WHITE, C_BLACK}Color;

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
        int w;
        int h;
    };
}Pairs;

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

#endif /* end of include guard: TYPES_H */
