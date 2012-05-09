sim: main.o core.o utils.o test.o
	cc -o sim main.o core.o utils.o test.o

main.o core.o utils.o test.o : utils.h logging.h

.PHONY: clean

clean:
	rm sim *.o

