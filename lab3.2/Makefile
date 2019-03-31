compile:
	gcc -c logger.c -o logger.o
	gcc -c base64.c -o base64.o
	gcc logger.o base64.o -o main.o

clean:
	rm logger.o
	rm base64.o
	rm main.o
	rm encoded.txt
	rm decoded.txt
