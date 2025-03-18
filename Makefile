TARGET = clocktimal
OBJS = clocktimal.o
CFLAGS = -Wall -I.
LIBS = 

all : ${TARGET}

${TARGET} : ${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LIBS}

clean:
	rm -f ${TARGET} ${OBJS}