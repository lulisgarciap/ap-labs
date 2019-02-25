#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#define size_buf 120

void file(char *ifp, char *ofp){
	int fi,fo;
	void *buf;
	fi = open(ifp, O_RDONLY);
	
	if(fi==-1){
		printf("open read error");
	}else{
		buf = (char*) malloc(size_buf);
		int c = read(fi, buf, size_buf);
		//printf("count: %d", c);
		//printf("%s", buf);
	}
	close(fi);
	fo = open(ofp, O_WRONLY| O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR) ;
	if(fo==-1){
		printf("open write error");	
	}else{
		int len = write ( fo, buf, size_buf ); 		}
	close(fo);				
}

int main(int argc, char *argv[]){
	clock_t begin = clock();
	file(argv[1], argv[2]);
	clock_t end  = clock();	
	printf("%6.6ld\n", end-begin);
	return 0;
}
