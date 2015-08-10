#########################################################
# File: Makefile
# # Description: A makefile for CS325 Project 4.
# #########################################################

CXX = gcc -std=c99 -Wall -lm

SRC1 = tsp_main.c

PROG1 = tsp_main 
PROGS = ${PROG1} 

all: program1

program1: ${SRC1}
	${CXX} ${SRC1} -o ${PROG1}  

#program4: ${SRC5}
#	${CXX} ${SRC5} ${SRC4} ${HEADERS} -o ${PROG4}  

#program5: ${SRC6}
#	${CXX} ${SRC6} -o ${PROG5}  

clean:
	rm -rf ${PROGS} *.tour *.o
