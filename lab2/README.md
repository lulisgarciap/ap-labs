gcc -c main.c -o main.o

gcc -c strlib.c -o strlib.o

gcc main.o strlib.o -o final.o
