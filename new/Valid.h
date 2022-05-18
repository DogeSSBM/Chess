#ifndef VALID_H
#define VALID_H

bool charValidAlpha(const char c)
{
    return c >= 'a' && c <= 'h';
}

bool charValidNum(const char c)
{
    return c >= '1' && c <= '8';
}

bool wcValidAlpha(const wc c)
{
    return c >= L'a' && c <= L'h';
}

bool wcValidNum(const wc c)
{
    return c >= L'1' && c <= L'8';
}

bool intValid(const int n)
{
    return n < 8 && n >= 0;
}

bool vecValid(const Vec pos, const bool throw)
{
    if(!intValid(pos.x) || !intValid(pos.y)){
        if(throw){
            fwprintf(stderr, L"Error: %ls out of bounds\n", vecStrify(pos));
            exit(EXIT_FAILURE);
        }else{
            return false;
        }
    }
    return true;
}

bool getValidAt(Valid moves, const Vec pos, const bool throw)
{
    vecValid(pos, throw);
    return moves[pos.y][pos.x];
}

void setValidAt(Valid moves, const Vec pos, const bool type, const bool throw)
{
    vecValid(pos, throw);
    moves[pos.y][pos.x] = type;
}

void resetValid(Valid moves)
{
    for(uint i = 0; i < 8; i++)
        memset(moves[i], false, sizeof(bool)*8);
}

uint validCount(Valid moves)
{
    uint total = 0;
    for(uint y = 0; y < 8; y++)
        for(uint x = 0; x < 8; x++)
            total += getValidAt(moves, (const Vec){.x = x, .y = y}, true);

    return total;
}

#endif /* end of include guard: VALID_H */
