CC = g++
CFLAGS = -Wall -c -g
OBJS = book.o main.o

hw1: ${OBJS}
	@echo
	@echo Linking $@
	${CC} -o $@ ${OBJS}

%.o: %.cpp
	@echo
	@echo Compiling $<
	${CC} ${CFLAGS} $< 2> $*.err
	@cat $*.err

clean:
	rm -f *.err *.o hw1
