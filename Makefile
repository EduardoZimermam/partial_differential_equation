CC = gcc -g

CFLAGS = -lm

objs = cgSolver.o functions.o utils.o

all: cgSolver

cgSolver: $(objs) $(CFLAGS)

utils.o: utils.c

functions.o: functions.c

cgSolver.o: cgSolver.c utils.o functions.o

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
	-rm -f $(objs) *~ cgSolver
	-rm -rf html 

purge:
	-rm -f $(objs) *~
