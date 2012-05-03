sim: main.o core.o utils.o
	cc -o sim main.o core.o utils.o

.PHONY: clean

clean:
	rm sim *.o

