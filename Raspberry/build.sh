#!/bin/sh

g++ -g -c -o rv_reg.o rv_reg.cpp
g++ -g -c -o rv.o rv.cpp
g++ -g -c -o main.o main.cpp
g++ -g -c -o rv_loop.o rv_loop.cpp

g++ -g -o rover rv.o main.o rv_loop.o rv_reg.o -lpthread
