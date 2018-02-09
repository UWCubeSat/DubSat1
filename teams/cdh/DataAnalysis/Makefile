# Make file to test dataArray
main: main.o dataArray.o
	gcc -Wall -std=c11 -g -o main main.o dataArray.o

main.o: main.c dataArray.h
	gcc -Wall -std=c11 -g -c main.c

dataArray.o: dataArray.c dataArray.h
	gcc -Wall -std=c11 -g -c dataArray.c

clean:
	rm -f *.o
