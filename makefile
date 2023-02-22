
src=$(wildcard *.c)
files=$(src:.c= )

build:$(files)
	@true

%:%.c
	gcc -Wall $< -o $@
