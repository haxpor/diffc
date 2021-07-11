.PHONY: all clean main.o

COMPILER=g++
CXX_FLAGS=-std=c++17 -Wall -Wextra -pedantic -g -O2 -fno-rtti
SRC_DIR=src
OUT=diffc

all: ${SRC_DIR}/main.o
	${COMPILER} $< -o ${OUT}

${SRC_DIR}/main.o: ${SRC_DIR}/main.cpp
	${COMPILER} ${CXX_FLAGS} -c $< -o $@

clean:
	rm -rf ${SRC_DIR}/*.o ${OUT}
