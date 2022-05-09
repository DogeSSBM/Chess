#ifndef DECLS_H
#define DECLS_H

wc* vec2Strify(const Vec2);
bool validPos(const Vec2, const bool);
void resetValid(Valid);
uint validMovesStateless(Board, Valid, const Vec2);
uint validMoves(Turn *, Valid, const Vec2);
bool getValidAt(Valid, const Vec2, const bool);
void setValidAt(Valid, const Vec2, const bool, const bool);
Turn* turnPosChanged(Turn *, const Vec2);
Color pieceColor(const Piece);
Piece pieceAt(Board, const Vec2);


#endif /* end of include guard: DECLS_H */
