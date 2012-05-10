CFLAGS := -DDEBUG

OBJS := main.o core.o utils.o test.o

sim: $(OBJS)
	cc $(CFLAGS) -o sim $(OBJS)

# dependencies on source files and header files
-include $(OBJS:.o=.d)

#gcc -MM will generate the dependency file list, which is used in the
#-include $(OBJS:.o=.d) statement to automatically determine the
#dependencies on header files (and c source files as well)
%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o
	gcc -MM $(CFLAGS) $*.c > $*.d


.PHONY: clean tags 

clean:
	rm sim $(OBJS) *.d

tags:
	find . -name "*.[ch]" | xargs etags -a
