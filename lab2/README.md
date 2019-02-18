gcc -c main.c -o main.o

gcc -c strlib.c -o strlib.o

gcc str-main.o strlib.o -o str-final.o
