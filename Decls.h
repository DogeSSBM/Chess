#ifndef DECLS_H
#define DECLS_H

wc* vec2Strify(const Vec2);
bool vecValid(const Vec2, const bool);
void resetValid(Valid);
uint validMovesStateless(Board, Valid, const Vec2);
uint validMoves(Turn *, Valid, const Vec2);
bool getValidAt(Valid, const Vec2, const bool);
void setValidAt(Valid, const Vec2, const bool, const bool);
Turn* turnPosChanged(Turn *, const Vec2);
Color pieceColor(const Piece);
Piece pieceAt(Board, const Vec2);
Vec2 getKing(Board, const Color);
bool inCheck(Board, const Color);
bool movesCanEscapeCheck(Board, Valid, const Vec2, const Color);
bool inCheckMate(Turn *, const Color);


#endif /* end of include guard: DECLS_H */
