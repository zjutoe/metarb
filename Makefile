sim: main.o core.o utils.o
	cc -o sim main.o core.o utils.o

main.o core.o utils.o : utils.h logging.h

.PHONY: clean

clean:
	rm sim *.o

