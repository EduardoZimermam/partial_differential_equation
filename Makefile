CFLAGS = -lm -O3 -mavx -march=native
LFLAGS = -DLIKWID_PERFMON -I /home/soft/likwid/include -L /home/soft/likwid/lib -llikwid

OBJS = pdeSolver.o functions.o utils.o

all: pdeSolver

pdeSolver: pdeSolver.o functions.o utils.o
	gcc pdeSolver.o functions.o utils.o -o pdeSolver $(CFLAGS) $(LFLAGS)

pdeSolver.o: pdeSolver.c functions.h
	gcc -c pdeSolver.c functions.h $(CFLAGS) $(LFLAGS)

functions.o: functions.c functions.h
	gcc -c functions.c $(LFLAGS)

utils.o: utils.c utils.h
	gcc -c utils.c

doc: Doxyfile
				doxygen $<

Doxyfile:
		doxygen -g
		sed -e "s;OPTIMIZE_OUTPUT_FOR_C *= *.*;OPTIMIZE_OUTPUT_FOR_C = YES;g" $@ >$@.new
		sed -e "s;EXTRACT_ALL *= *.*;EXTRACT_ALL = YES;g" $@.new >$@
		sed -e "s;EXTRACT_PRIVATE *= *.*;EXTRACT_PRIVATE = YES;g" $@ >$@.new
		sed -e "s;EXTRACT_STATIC *= *.*;EXTRACT_STATIC = YES;g" $@.new >$@
		sed -e "s;EXTRACT_LOCAL_METHODS *= *.*;EXTRACT_LOCAL_METHODS = YES;g" $@ >$@.new
		sed -e "s;GENERATE_LATEX *= *.*;GENERATE_LATEX = NO;g" $@.new >$@
		sed -e "s;SOURCE_BROWSE *= *.*;SOURCE_BROWSE = YES;g" $@ >$@.new
		sed -e "s;VERBATIM_HEADER *= *.*;VERBATIM_HEADER = YES;g" $@.new >$@
		rm -f $@.new

clean:
	-rm -f $(OBJS) *~ pdeSolver
	-rm -rf html

purge:
	-rm -f $(OBJS) *~
