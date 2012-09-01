CFLAGS := -DDEBUG

OBJS := main.o core.o mem.o utils.o test.o

sim: $(OBJS)
	cc $(CFLAGS) -o sim $(OBJS) -ldl -lm -llua

# dependencies on source files and header files
-include $(OBJS:.o=.d)

#gcc -MM will generate the dependency file list, which is used in the
#-include $(OBJS:.o=.d) statement to automatically determine the
#dependencies on header files (and c source files as well)
%.o: %.c
	gcc -c $(CFLAGS) $*.c -o $*.o
	gcc -MM $(CFLAGS) $*.c > $*.d

inst_dispatch_switch.h: inst.h
	./gen_dispatch.sh > inst_dispatch_switch.h


.PHONY: clean tags

clean:
	rm sim $(OBJS) *.d

distclean:
	rm sim $(OBJS) *.d TAGS cscope.*

tags:
	find . -name "*.[ch]" > cscope.files
	cscope -bq
	cat cscope.files | xargs etags -a

