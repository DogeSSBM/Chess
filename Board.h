#ifndef BOARD_H
#define BOARD_H

size_t strnlen(const char* str, const size_t max)
{
  const char* end = memchr(str, '\0', max);
  return end ? (size_t)(end-str) : max;
}

bool pairuInBounds(const Pairu target)
{
    return target.x < 8 && target.y < 8;
}

bool isValidAlpha(const wc c)
{
    return c >= L'a' && c <= L'h';
}

bool isValidNum(const wc c)
{
    return c >= L'0' && c <= L'8';
}

uint coordToUint(const wc c)
{
    if(isValidNum(c))
        return L'8' - c;
    if(isValidAlpha(c))
        return c - L'a';
    return 9;
}

wc getAt(Board board, const Pairu target)
{
    if(!pairuInBounds(target)){
        fwprintf(stderr, L"Error: target (%u, %u) out of bounds\n", target.x, target.y);
        exit(EXIT_FAILURE);
    }
    return board[target.y][target.x];
}

wc setAt(Board board, const Pairu target, const wc piece)
{
    if(!pairuInBounds(target)){
        fwprintf(stderr, L"Error: target (%u, %u) out of bounds\n", target.x, target.y);
        exit(EXIT_FAILURE);
    }
    const wc prev = getAt(board, target);
    board[target.y][target.x] = piece;
    return prev;
}

Color pieceColor(const wc piece)
{
    if(
        piece == L'♜' || piece == L'♞' || piece == L'♝' ||
        piece == L'♛' || piece == L'♚' || piece == L'♟'
    )
        return C_WHITE;

    if(
        piece == L'♖' || piece == L'♘' || piece == L'♗' ||
        piece == L'♕' || piece == L'♔' || piece == L'♙'
    )
        return C_BLACK;

    if(piece != L' '){
        fwprintf(stderr, L"Error: \'%lc\' is not a valid piece\n", piece);
        exit(EXIT_FAILURE);
    }

    return C_NONE;
}

void printMove(const Move move)
{
    if(move.type == M_HALF)
        wprintf(L"src: (%u, %u)\n", move.src.x, move.src.y);
    else if(move.type == M_VALID)
        wprintf(L"src: (%u, %u) dst: (%u, %u)\n", move.src.x, move.src.y, move.dst.x, move.dst.y);
    else
        wprintf(L"M_INVALID\n");
}

bool moveInBounds(const Move move)
{
    return pairuInBounds(move.src) && pairuInBounds(move.dst);
}

Move tryReadMove(Move move)
{
    char buf[6] = {'\0'};
    if(!fgets(buf, 6, stdin)){
        fwprintf(stderr, L"Error: input\n");
        exit(EXIT_FAILURE);
    }
    const uint i = strnlen(buf, 6);

    if(buf[i] == '\n')
        while(fgetc(stdin) != '\n');


    if(i-1 == 2){
        if(move.type == M_INVALID){
            move.src.x = coordToUint(buf[0]);
            move.src.y = coordToUint(buf[1]);
            move.type = pairuInBounds(move.src) ? M_HALF : M_INVALID;
        }else if(move.type == M_HALF){
            move.dst.x = coordToUint(buf[0]);
            move.dst.y = coordToUint(buf[1]);
            move.type = moveInBounds(move) ? M_VALID : M_INVALID;
        }
    }else if(i-1 == 4){
        if(move.type == M_INVALID){
            move.src.x = coordToUint(buf[0]);
            move.src.y = coordToUint(buf[1]);
            move.dst.x = coordToUint(buf[2]);
            move.dst.y = coordToUint(buf[3]);
            move.type = moveInBounds(move) ? M_VALID : M_INVALID;
        }
    }else{
        move.type = M_INVALID;
    }

    return move;
}

#endif /* end of include guard: BOARD_H */
