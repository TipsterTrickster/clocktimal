TARGET = clocktimal
OBJS = clocktimal.o solvers.o utils.o
CFLAGS = -Wall -I. -pthread
LIBS =

all : ${TARGET}

${TARGET} : ${OBJS}
	${CC} ${LIBS} -o ${TARGET} ${OBJS} 

clean:
	rm -f ${TARGET} ${OBJS}