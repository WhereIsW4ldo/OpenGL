

all: main(16)

main: main(16).c 
	gcc -o main main(16).c all.h
