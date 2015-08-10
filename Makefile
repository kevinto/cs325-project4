#########################################################
# File: Makefile
# # Description: A makefile for CS325 Project 4.
# #########################################################

CXX = gcc -std=c99 -lm

SRC1 = tspalgo.c
SRC4 = filefunctions.c

PROG1 = tsp 
PROGS = ${PROG1} 

HEADER1 = filefunctions.h
HEADERS = ${HEADER1}

all: program1

program1: ${SRC1}
	${CXX} ${SRC1} -lm ${SRC4} ${HEADERS} -o ${PROG1}  

#program4: ${SRC5}
#	${CXX} ${SRC5} ${SRC4} ${HEADERS} -o ${PROG4}  

#program5: ${SRC6}
#	${CXX} ${SRC6} -o ${PROG5}  

clean:
	rm -rf ${PROGS} *.tour