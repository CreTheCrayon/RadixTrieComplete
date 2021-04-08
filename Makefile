.SUFFIXES: .c .o

CC = gcc

CCFLAGS = -std=c99 -pedantic -Wall

EXEC = main

OBJS = main.o functions.o


${EXEC}: ${OBJS}

		${CC} ${CCFLAGS} -o ${EXEC} ${OBJS}


.c.o:

		${CC} ${CCFLAGS} -c $<


run: ${EXEC}

		./${EXEC}


clean:

		rm -f ${EXEC}
		rm -f *.o


main.o: main.c functions.h

functions.o: functions.c functions.h
