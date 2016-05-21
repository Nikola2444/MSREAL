all:calculate
calculate:calculate.o helper.o
	gcc -o calculate calculate.o helper.o
calculate.o:calculate.c helper.h
	gcc -c calculate.c helper.h
helper.o:helper.c helper.h
	gcc -c helper.c helper.h



clean:
	rm calculate
	rm *.o
