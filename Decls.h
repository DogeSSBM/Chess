#ifndef DECLS_H
#define DECLS_H

wc* vecStrify(const Vec);
bool validPos(const Vec, const bool);
void applyMove(Board, Turn);
bool getValidAt(Valid, const Vec, const bool);
void setValidAt(Valid, const Vec, const bool, const bool);
bool inCheck(Board, const Color);
Turn* lastTurn(Turn *);
GameState consGameState(Turn *);
bool inCheck(Board, const Color);
Color stateTypeColor(const GameStateType);
void boardStrify(Board, BoardStr);

#endif /* end of include guard: DECLS_H */
