#ifndef BUFFER_H

Buffer* bufCat(Buffer *buffer, wc* str)
{
    if(!buffer)
        buffer = calloc(1, sizeof(Buffer));
    if(!buffer->str){
        buffer->str = calloc(2048, sizeof(wc));
        buffer->end = buffer->str + 2048;
        buffer->pos = buffer->str;
        memset(buffer->str, L'\0', sizeof(wc)*2048);
    }
    if(!str)
        return buffer;
    if(buffer->pos + wcslen(str) >= buffer->end){
        fwprintf(stderr, L"Error: bufCat would exceed buffer len\n");
        fwprintf(stderr, L"Buffer -\n%ls\n\nstr -\n%ls\n", buffer->str, str);
        exit(EXIT_FAILURE);
    }
    wcscat(buffer->pos, str);
    buffer->pos += wcslen(str);
    return buffer;
}

Buffer* bufCatf(Buffer *buffer, wc* format, ...)
{
    va_list args;
    va_start(args, format);

    va_list args2;
    va_copy(args2, args);
    wc buf[vswprintf(NULL, 0, format, args) + 1];

    vswprintf(buffer->pos, sizeof buf, format, args2);
    va_end(args);
    va_end(args2);
    return buffer;
}

Buffer* bufClear(Buffer *buffer)
{
    if(!buffer){
        fwprintf(stderr, L"Error: cannot bufClear when buffer is NULL\n");
        exit(EXIT_FAILURE);
    }
    if(!buffer->str){
        fwprintf(stderr, L"Error: cannot bufClear when buffer->str is NULL\n");
        exit(EXIT_FAILURE);
    }
    memset(buffer->str, L'\0', buffer->end-buffer->str);
    buffer->pos = buffer->str;
    return buffer;
}

Buffer* bufBoard(Buffer *buffer)
{
    Board board;
    memcpy(board[0], (wc[8]){L'♖',L'♘',L'♗',L'♕',L'♔',L'♗',L'♘',L'♖'}, sizeof(wc)*8);
    memcpy(board[1], (wc[8]){L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙',L'♙'}, sizeof(wc)*8);
    memcpy(board[2], (wc[8]){L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '}, sizeof(wc)*8);
    memcpy(board[3], (wc[8]){L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '}, sizeof(wc)*8);
    memcpy(board[4], (wc[8]){L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '}, sizeof(wc)*8);
    memcpy(board[5], (wc[8]){L' ',L' ',L' ',L' ',L' ',L' ',L' ',L' '}, sizeof(wc)*8);
    memcpy(board[6], (wc[8]){L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟',L'♟'}, sizeof(wc)*8);
    memcpy(board[7], (wc[8]){L'♜',L'♞',L'♝',L'♛',L'♚',L'♝',L'♞',L'♜'}, sizeof(wc)*8);

    if(buffer)
        bufClear(buffer);
    bufCat(buffer, L"    a   b   c   d   e   f   g   h  \n");
    bufCatf(buffer, L"  +---+---+---+---+---+---+---+---+\n");
    for(int y = 0; y < 8; y++){
        bufCatf(buffer, L"%lc |", btowc('8'-y));
        for(int x = 0; x < 8; x++){
            // const Pairu src = {.x = x, .y = y};
            bufCatf(buffer, L" %lc |", btowc('8'-y));
        }
        bufCatf(buffer, L"%lc |", btowc('8'-y));
    }
    return buffer;
}

#define BUFFER_H
#endif /* end of include guard: BUFFER_H */
