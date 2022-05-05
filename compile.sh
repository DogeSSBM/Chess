#!/bin/sh
clear
flags="-std=c11 -Wall -Wextra -Wpedantic -g"
cc main.c $flags -o main.out
