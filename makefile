
src=$(wildcard src/*.c)
dst=bin/myshell

$(dst):$(src)
	@mkdir -p bin
	gcc -Wall $(src) -o $(dst)

run:$(dst)
	@$(dst)
