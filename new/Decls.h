#ifndef DECLS_H
#define DECLS_H

wc* vecStrify(const Vec);
bool getValidAt(Valid, const Vec, const bool);
void setValidAt(Valid, const Vec, const bool, const bool);
bool inCheck(Board, const Color);
Turn* lastTurn(Turn *);
GameState consGameState(Turn *);


#endif /* end of include guard: DECLS_H */
