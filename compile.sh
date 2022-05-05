#!/bin/sh
clear
flags="-std=c11 -Wall -Wextra -Wpedantic"
cc main.c $flags -o main.out
