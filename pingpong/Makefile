MPICC=/home/shiroha/mpich/mpich-4.0.2/bin/mpicc
MPIRUN=/home/shiroha/mpich/mpich-4.0.2/bin/mpirun
EXEC=main

all:
	${MPICC} -o ${EXEC} main.c

run:
	make
	${MPIRUN} -np 2 ./${EXEC}

