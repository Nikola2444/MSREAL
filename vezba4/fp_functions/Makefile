CFLAGS = -g

all:fp_example
fp_example: fp_example.o
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rfv *.o
	rm -rfv example

.PHONY: clean
