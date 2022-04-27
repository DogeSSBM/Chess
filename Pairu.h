#ifndef PAIRU_H
#define PAIRU_H

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

#endif /* end of include guard: PAIRU_H */
