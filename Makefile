CFLAGS := -DDEBUG

sim: main.o core.o utils.o test.o
	cc $(CFLAGS) -o sim main.o core.o utils.o test.o

main.o core.o utils.o test.o : utils.h logging.h inst.h test.h

.PHONY: clean

clean:
	rm sim *.o

