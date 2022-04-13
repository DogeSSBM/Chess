#ifndef IO_H
#define IO_H

void clearTerm(void)
{
    wprintf(__extension__(L"\e[1;1H\e[2J"));
}

#endif /* end of include guard: IO_H */
