gcc -c testLogger.c -o testlog.o
gcc -c logger.c -o logger.o
gcc logger.o testlog.o -o main.o
./main.o
