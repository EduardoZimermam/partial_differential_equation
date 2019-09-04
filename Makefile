
    CC     = gcc -g

    CFLAGS = -std=c11
    LFLAGS = -lm

      PROG = pdeSolver
      OBJS = utils.o

%.o: %.c %.h utils.h
	$(CC) -c $(CFLAGS) $<

$(PROG):  $(OBJS) $(PROG).o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f $(PROG)