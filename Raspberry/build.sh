#!/bin/sh

g++ -g -c -o rover.o rover.cpp
g++ -g -c -o main.o main.cpp
g++ -g -c -o rv_loop.o rv_loop.cpp

g++ -g -o rover rover.o main.o rv_loop.o -lpthread
