TARGET = clocktimal
OBJS = clocktimal.o solvers.o utils.o
TARGET2 = movecountstats
OBJS2 = movecountstats.c solvers.o utils.o
CFLAGS = -Wall -I. -pthread
LIBS =




all : ${TARGET} ${TARGET2}

${TARGET} : ${OBJS}
	${CC} ${LIBS} -o ${TARGET} ${OBJS} 

${TARGET2} : ${OBJS2}
	${CC} ${LIBS} -o ${TARGET2} ${OBJS2} 

clean:
	rm -f ${TARGET} ${OBJS}