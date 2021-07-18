.PHONY: all clean install main.o

COMPILER=g++
CXX_FLAGS=-std=c++17 -Wall -Wextra -pedantic -Wfatal-errors -g -O2 -fno-rtti
SRC_DIR=src
OUT=diffp4

all: ${SRC_DIR}/main.o
	${COMPILER} $< -o ${OUT}

${SRC_DIR}/main.o: ${SRC_DIR}/main.cpp ${SRC_DIR}/ColorifyText.h
	${COMPILER} ${CXX_FLAGS} -c $< -o $@

install: all 
	cp -av ${OUT} /usr/local/bin/	

clean:
	rm -rf ${SRC_DIR}/*.o ${OUT}
