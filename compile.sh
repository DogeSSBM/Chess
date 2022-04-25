#!/bin/sh
clear
flags="-std=c11 -Wall -Wpedantic"
cc main.c $flags -o main.out
