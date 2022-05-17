#ifndef Vec2_H
#define Vec2_H

Vec VecAbsDif(const Vec p1, const Vec p2)
{
    return (const Vec){
        .x = maxu(p1.x, p2.x) - minu(p1.x, p2.x),
        .y = maxu(p1.y, p2.y) - minu(p1.y, p2.y)
    };
}

bool vecEq(const Vec p1, const Vec p2)
{
    return p1.x == p2.x && p1.y == p2.y;
}
/*    7  8  1
      15 0 9    UL U UR
        \|/       \|/
    6 14-+-10 2  L-+-R
        /|\       /|\
     13 12 11   DL D DR
      5  4  3        */
Dir dirRor(const Dir dir)
{
    return (dir+1)%D_N;
}

Dir dirRoR(const Dir dir)
{
    return (dir+2)%D_N;
}

Dir dirRol(const Dir dir)
{
    return (dir+7)%D_N;
}

Dir dirRoL(const Dir dir)
{
    return (dir+6)%D_N;
}

Dir dirInv(const Dir dir)
{
    return (dir+4)%D_N;
}

int dirX(const Dir dir)
{
    if(dir == D_L || dir == D_UL || dir == D_DL )
        return -1;
    if(dir == D_R || dir == D_UR || dir == D_DR )
        return 1;
    return 0;
}

int dirY(const Dir dir)
{
    if(dir == D_U || dir == D_UL || dir == D_UR )
        return -1;
    if(dir == D_D || dir == D_DL || dir == D_DR )
        return 1;
    return 0;
}

Vec off(const Vec pos, const Vec off)
{
    return (const Vec){.x = pos.x+off.x, .y = pos.y+off.y};
}

Vec shift(const Vec pos, const Dir dir, const uint dist)
{
    // wprintf(L"%ls: (%i, %i)\n", DirStr[dir], dirX(dir), dirY(dir));
    return off(pos, (const Vec){.x = dirX(dir)*dist, .y = dirY(dir)*dist});
}

#endif /* end of include guard: Vec2_H */
