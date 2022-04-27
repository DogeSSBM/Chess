#ifndef DIR_H
#define DIR_H

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

#endif /* end of include guard: DIR_H */
