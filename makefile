
src=$(wildcard src/*.c)
dst=bin/myshell

$(dst):$(src)
	gcc -Wall $(src) -o $(dst)

run:$(dst)
	@$(dst)