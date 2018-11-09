CFLAGS = -g

all: example
pwm_button: pwm_button.o
	${CC} ${CFLAGS} $^ -o $@

clean:
	rm -rfv *.o
	rm -rfv example

.PHONY: clean
